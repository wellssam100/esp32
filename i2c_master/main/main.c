//Skkeleton main.c file

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c_master.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"


/**   
 * @param pvParameters Pointer to a parameter array, I need to pass in a master device handle
 *
 * 
 * 
 * 
 *  
 */

void send_data(void *pvParameters){
    uint8_t data =54;
    uint8_t *data_ptr = &data;
    i2c_master_dev_handle_t *dev_handle = (i2c_master_dev_handle_t *)pvParameters;
    while(1){
        printf("Sending Data\n");
        esp_err_t ret = i2c_master_transmit(*dev_handle, data_ptr, sizeof(pvParameters), 1000 / portTICK_PERIOD_MS);
    }
}

void receive_data(void *pvParameters){
    uint8_t data =14;
    uint8_t *data_ptr = &data;
    i2c_master_dev_handle_t *dev_handle = (i2c_master_dev_handle_t *)pvParameters;
    while(1){
        printf("Receiving Data: %u\n", data);
        esp_err_t ret = i2c_master_receive(*dev_handle, data_ptr, sizeof(pvParameters), 1000 / portTICK_PERIOD_MS);
        printf("Received Data: %u\n", data);
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
    (i2c_new_master_bus(&bus_conf, &bus_handle));
    return bus_handle; 
}

i2c_master_dev_handle_t setup_device_i2c(i2c_addr_bit_len_t addr_bit_len, uint16_t dev_addr, i2c_master_bus_handle_t bus_handle){
    i2c_master_dev_handle_t dev_handle;
    i2c_device_config_t dev_conf = {
        .dev_addr_length = addr_bit_len,
        .device_address = dev_addr,
        .scl_speed_hz = 400000, 
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_conf, &dev_handle));
    return dev_handle;
}

void app_main(void) {
    //Setup master bus to be a global variable so that I don't have to pass it to the slave dev
    i2c_master_bus_handle_t master_bus = setup_master_i2c();
    i2c_master_dev_handle_t master_dev_handle = setup_device_i2c(I2C_ADDR_BIT_7, 0x58, master_bus);
    // pointer to task function, name for task (debug), stack size in bytes for task, parameters to pass to the task, task priority, handle to the task
    //xTaskCreate(&sample_task, "Sample", 2048, NULL, 1, NULL);
    //need to add parameters through *pvParameters
    
    xTaskCreate(&send_data, "Send", 2048, (void *) master_dev_handle, 1, NULL);
}