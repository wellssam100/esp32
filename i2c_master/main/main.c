//Note 1-6 is master, 1-5 is slave

//Includes

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "esp_err.h"


//Variable Definitions

#define I2C_MASTER_SCL_IO 22               
#define I2C_MASTER_SDA_IO 21          
#define I2C_MASTER_FREQ_HZ 100000
#define I2C_SLAVE_ADDRESS 0x42         
#define I2C_MASTER_TIMEOUT_MS 1000


//Constants

static const char *TAG = "I2C_MASTER";

static i2c_master_bus_handle_t master_bus_handle;
static i2c_master_dev_handle_t master_dev_handle;


//Functions

void i2c_master_task(void *arg) {
    uint8_t data_to_send[2] = {0xA5, 0x5A}; // Example data
    esp_err_t ret;

    while (1) {
        ESP_LOGI(TAG, "Attempting to communicate with slave...");
        ret = i2c_master_transmit(master_dev_handle, data_to_send, sizeof(data_to_send), pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));

        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Data sent successfully!");
        } else {
            ESP_LOGE(TAG, "Failed to send data, error: %s", esp_err_to_name(ret));
            ESP_LOGI(TAG, "Retrying in 10 seconds...");
        }

        vTaskDelay(pdMS_TO_TICKS(10000)); // Wait 10 seconds before retrying
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "Initializing I2C master...");

    // Configure master bus
    i2c_master_bus_config_t bus_config = {
        .sda_io_num = 18,       
        .scl_io_num = 19,      
        .flags={
            .enable_internal_pullup = true
        },
        .clk_source = I2C_CLK_SRC_DEFAULT,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &master_bus_handle));

    // Configure master device
    i2c_device_config_t master_device_config = {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = I2C_SLAVE_ADDRESS,
        .scl_speed_hz = 400000, 
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(master_bus_handle, &master_device_config, &master_dev_handle));

    ESP_LOGI(TAG, "I2C master initialized!");

    // Create FreeRTOS task for I2C communication
    xTaskCreate(i2c_master_task, "i2c_master_task", 2048, NULL, 10, NULL);
}