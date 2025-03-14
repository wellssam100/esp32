/**
 * @file Source for oled 
 */
#include <stdio.h>
#include "include/oled.h"
#include <esp_err.h>
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_system.h"
#include "esp_log.h"
#include <stdint.h>


#define TEST_LCD_PIXEL_CLOCK_HZ (400 * 1000)
#define HEIGHT 64
#define WIDTH 128


const uint8_t letterData[][8] = {
    {
        0b00011000,
        0b00100100,
        0b01000010,
        0b00011000,
        0b00100100,
        0b01000010,
        0b01010101,
        0b10101010
    } 
};

//init the device

esp_err_t oled_dev_init(oled_dev_t* oled_dev, i2c_master_bus_handle_t* master_bus, uint32_t oled_i2c_addr)
{
    //Check to see if the passed arg is null
    // I should only need 2 pins, I am using i2c to communicate with the board
    if(!(oled_dev)||!(master_bus)){
        return ESP_ERR_INVALID_ARG;
    }
    //There is a handle and a config for both the io and the panel, all 4 need to be setup  
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = oled_i2c_addr,
        .scl_speed_hz = 1000,//TEST_LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1, // According to SSD1306 datasheet
        .dc_bit_offset = 6,       // According to SSD1306 datasheet
        .lcd_cmd_bits = 8,        // According to SSD1306 datasheet
        .lcd_param_bits = 8,      // According to SSD1306 datasheet
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(*master_bus, &io_config, &io_handle));
        
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = -1,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    //set up struct
    oled_dev->i2c_config = io_config;
    oled_dev->i2c_handle = io_handle;
    oled_dev->panel_config = panel_config;
    oled_dev->panel_handle = panel_handle;
    oled_dev->max_height = HEIGHT;
    oled_dev->max_width = WIDTH;
    
    
    //prep screen
    ESP_LOGI("OLED", "%p",panel_handle);
    ESP_LOGI("OLED", "%p",oled_dev->panel_handle);

    return ESP_OK;
    
}

esp_err_t oled_draw_dot(const oled_dev_t* oled_dev, uint8_t x, uint8_t y){
    static uint16_t s_white_buffer[WIDTH *HEIGHT] ;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        s_white_buffer[i] = 0xFFFF;  // White pixel in RGB565
    }
    static const uint16_t s_dot_data[2] = {
        0x0003,0x0003
    };

    static uint16_t s_black_buffer[2] = {0};
    //ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x_start,y_start,x_start+2,y_start+2,s_dot_data));
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x,y,x+2,y+4,&s_dot_data));
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x-2,y,x,y+4,&s_black_buffer));
    //ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x,y,x+2,y+4,&s_black_buffer));

    return ESP_OK;
}

esp_err_t oled_draw_letter(const oled_dev_t* oled_dev, uint8_t x, uint8_t y){
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x,y,x+8,y+8,&letterData[0]));
    return ESP_OK;
}