//Skkeleton main.c file

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/i2c_master.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"
//For ESP_GOTO_ON_FALSE
#include "esp_check.h"

SemaphoreHandle_t i2c_mutex;

/**   
 * @param pvParameters Pointer to a parameter array, I need to pass in a master device handle and the master device bus for testing
 *
 * 
 * 
 * 
 *  
 */

void send_data(void *pvParameters){
    static uint8_t write_buffer[] = {0xA0, 0xB1, 0xC2};
    i2c_master_dev_handle_t *dev_handle = (i2c_master_dev_handle_t *)pvParameters;    
    
    while(1){
        ESP_DRAM_LOGI("Example", "Device handle pointer: %p\nWrite pointer: %p", (void *)dev_handle, (void *)write_buffer);
     
        if (dev_handle == NULL || *dev_handle == NULL) {
            ESP_DRAM_LOGI("Example", "Invalid device handle\n");
            abort();  // Stop execution if the handle is invalid
        }
        
        //Take the I2C    
        if(xSemaphoreTake(i2c_mutex, portMAX_DELAY) == pdTRUE){
            //Send data
            i2c_master_tr
            esp_err_t ret = i2c_master_transmit(*dev_handle, write_buffer, sizeof(write_buffer), pdMS_TO_TICKS(500));
            if (ret != ESP_OK){
                ESP_DRAM_LOGI("Example", "Failed, Error Code:%x\n", ret);
                
            }  
            else{
                ESP_DRAM_LOGI("Example", "Sent, Error Code:%x\n", ret);
            }
            //Release semaphore
            xSemaphoreGive(i2c_mutex);
            //delay
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void receive_data(void *pvParameters){
    uint8_t data =14;
    uint8_t *data_ptr = &data;
   
    i2c_master_dev_handle_t *dev_handle = (i2c_master_dev_handle_t *)pvParameters;
    while(1){
        ESP_DRAM_LOGI("Example", "Receiving Data: %u\n", data);
        esp_err_t ret = i2c_master_receive(*dev_handle, data_ptr, sizeof(pvParameters), 1000 / portTICK_PERIOD_MS);
        ESP_DRAM_LOGI("Example", "Received Data: %u\n", data);
    }
}

i2c_master_bus_handle_t setup_master_i2c(){
    i2c_master_bus_handle_t bus_handle;
    i2c_master_bus_config_t bus_conf = {
        .sda_io_num = 18,       
        .scl_io_num = 19,      
        .flags={
            .enable_internal_pullup = true
        },
        .clk_source = I2C_CLK_SRC_DEFAULT,
    };
    printf("add master bus\n");
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_conf, &bus_handle));


    return bus_handle; 
}

esp_err_t  setup_device_i2c(i2c_addr_bit_len_t addr_bit_len, uint16_t dev_addr, i2c_master_bus_handle_t bus_handle){
    //Start with Error object to return
    esp_err_t ret = ESP_OK;
    
    //Create pointer to master device definition
    //Check to see if there is memory space for the device

    //Handle for master device, shares bus information with master bus
    i2c_master_dev_handle_t master_dev_handle = (i2c_device_config_t)calloc(1,sizeof(*master_dev_handle));
    ESP_GOTO_ON_FALSE(master_dev_handle, ESP_ERR_NO_MEM, err, "MEM ERR", "no memory for the i2c device");

    i2c_device_config_t master_dev_conf = {
        .dev_addr_length = addr_bit_len,
        .device_address = dev_addr,
        .scl_speed_hz = 400000, 
    };
    
    //This if might not be necessary, the master_dev_handle should always be null....
    //if (master_dev_handle == NULL){
        ESP_GOTO_ON_ERROR(i2c_master_bus_add_device(bus_handle, &master_dev_conf, &master_dev_handle), err, "DEV ERR", "i2c new bus failed");
    //}

    return ret;

err:
    if(master_dev_handle){
        i2c_master_bus_rm_device(master_dev_handle);
    }
    free(master_dev_handle);
    return ret;
}

void app_main(void) {
    //init semaphore
    i2c_mutex = xSemaphoreCreateMutex();
    if (i2c_mutex == NULL){
        printf("failed to create i2c mutex\n");
        abort();
    }
    //Setup master bus to be a global variable so that I don't have to pass it to the slave dev
    i2c_master_bus_handle_t master_bus = setup_master_i2c();
    i2c_master_dev_handle_t master_dev_handle = setup_device_i2c(I2C_ADDR_BIT_7, 0x58, master_bus);
    // pointer to task function, name for task (debug), stack size in bytes for task, parameters to pass to the task, task priority, handle to the task
    //xTaskCreate(&sample_task, "Sample", 2048, NULL, 1, NULL);
    //need to add parameters through *pvParameters
    
    xTaskCreate(&send_data, "Send", 4096, &master_dev_handle, 1, NULL);
}