
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// Controller Headers
#include "driver/i2c_master.h"
#include "esp_log.h"
//Info Get
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_flash.h"


void get_info (){
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ", 
            CONFIG_IDF_TARGET,
            chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");
    
    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);

    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }
    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
}

i2c_master_bus_handle_t setup_bus(void){
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

void scan_i2c_devices(i2c_master_bus_handle_t bus_handle){
    int dev_count = 0;

    for (int addr =0; addr <127; addr++){
        i2c_device_config_t dev_conf = {
          .dev_addr_length = I2C_ADDR_BIT_LEN_7,
          .device_address = addr,
          .scl_speed_hz = 400000, 
        };
        uint8_t write_buf[2] = {addr, 1};
        i2c_master_dev_handle_t dev_handle;        
        i2c_master_bus_add_device(bus_handle, &dev_conf, &dev_handle);
        esp_err_t ret = i2c_master_transmit(dev_handle, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS);
        if (ret == ESP_OK){
            dev_count++;
            printf("Dev at addr 0x%x\n", addr);
        }
        //ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_conf, &dev_handle));
        //ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS));

    }
    printf("%d Devices found\n", dev_count);
    return;
}

void scan_single_dev(i2c_master_bus_handle_t bus_handle){
    uint8_t write_buf[2] = {0x58, 1};
    
    i2c_master_dev_handle_t dev_handle;
    i2c_device_config_t dev_conf = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x58,
        .scl_speed_hz = 400000, 
    };
    printf("add device\n");
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_conf, &dev_handle));
    printf("transmit\n");
    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, write_buf, sizeof(write_buf), 1000 / portTICK_PERIOD_MS));

}



void app_main(void) {
    // Get information about he device
    esp_log_level_set("*", ESP_LOG_NONE);  // Set log level for all components
    get_info();
    i2c_master_bus_handle_t bus_handle = setup_bus();
    //scan_single_dev(bus_handle);
    scan_i2c_devices(bus_handle);
}