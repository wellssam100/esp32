/**
 * @file oled.h 
 * This library should allow users to draw basic shapes onto an oled
 * Create a device descriptor for ssd1036
 */
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_system.h"
#include <stdio.h>
#include <stdint.h>


typedef struct oled_dev_t{
    //I2C handles
    int i2c_addr; //device address
    esp_lcd_panel_io_handle_t i2c_handle; 
    esp_lcd_panel_io_i2c_config_t i2c_config;
    esp_lcd_panel_handle_t panel_handle;
    esp_lcd_panel_dev_config_t panel_config;
    //Screen stuff
    int max_height;//64
    int max_width;//128s
} oled_dev_t;

/**
 * @brief initialize the i2c and panel configurations
 * @param oled_dev Oled Device structure
 * @return ESP_OK on success, fail if any failed pointers
 */
esp_err_t oled_dev_init(oled_dev_t* oled_dev, i2c_master_bus_handle_t* master_bus, uint32_t oled_i2c_addr);

//Do I need to initialize the aspect ratio? Range for x and y?


/**
 * @
 * 
 */
esp_err_t oled_draw_dot(const oled_dev_t* oled_dev, uint8_t x, uint8_t y);

esp_err_t oled_draw_letter(const oled_dev_t* oled_dev, uint8_t x, uint8_t y);