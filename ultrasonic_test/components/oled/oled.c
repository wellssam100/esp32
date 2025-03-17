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


uint8_t font[][8] = {
    {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
    },
    {
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111
    },
    {
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101
    },
    {
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010,
        0b01010101,
        0b10101010
    },
    //0
    {
        0b01111100,
        0b11000110,
        0b11001110,
        0b11011110, 
        0b11110110,
        0b11100110,
        0b01111100,
        0b00000000
    },
    //1
    {
        0b00111000,
        0b01111000,
        0b00111000,
        0b00111000,
        0b00111000,
        0b00111000,
        0b11111110,
        0b00000000
    },
    //2
    {
        0b01111100,
        0b11000110,
        0b00000110,
        0b00011100,
        0b01100000,
        0b11001110,
        0b11111110,
        0b00000000
    },
    //3
    {
        0b01111100,
        0b11000110,
        0b00000110,
        0b00111100,
        0b00000110,
        0b11000110,
        0b01111100,
        0b00000000
    },
    //4
    {
        0b00011100,
        0b00111100,
        0b01101100,
        0b11001100,
        0b11111110,
        0b00001100,
        0b00011110,
        0b00000000
    },
    //5
    {
        0b11111110,
        0b11000000,
        0b11111100,
        0b00000110,
        0b00000110,
        0b11000110,
        0b01111100,
        0b00000000
    },
    //6
    {
        0b00111100,
        0b01100000,
        0b11000000,
        0b11111100,
        0b11000110,
        0b11000110,
        0b01111100,
        0b00000000
    },
    //7
    {
        0b11111110,
        0b11000110,
        0b00000110,
        0b00001100,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00000000
    },
    //8
    {
        0b01111100,
        0b11000110,
        0b11000110,
        0b01111100,
        0b11000110,
        0b11000110,
        0b01111100,
        0b00000000
    },
    //9
    {
        0b01111100,
        0b11000110,
        0b11000110,
        0b01111110,
        0b00000110,
        0b00001100,
        0b01111000,
        0b00000000
    },
    //A
    {
        0b00111000,
        0b01111100,
        0b11000110,
        0b11000110,
        0b11111110,
        0b11000110,
        0b11000110,
        0b00000000
    },
    //B
    {
        0b11111100,
        0b01100110,
        0b01100110,
        0b01111100,
        0b01100110,
        0b01100110,
        0b11111100,
        0b00000000
    },
    //C
    {
        0b01111100,
        0b11000110,
        0b11000000,
        0b11000000,
        0b11000000,
        0b11000110,
        0b01111100,
        0b00000000
    },
    //D
    {
        0b11111000,
        0b01101100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01101100,
        0b11111000,
        0b00000000
    },
};
uint8_t num_font = sizeof(font) / sizeof(font[0]);
static uint16_t s_white_buffer[WIDTH *HEIGHT] ;
static const uint16_t s_dot_data[2] = {
    0x0003,0x0003
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
        .scl_speed_hz = 2000,//TEST_LCD_PIXEL_CLOCK_HZ,
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
    int i=0;
    int len = sizeof(font) / sizeof(font[0]);
    for(i=0;i<len;i++){
        rotate8x8(font[i], 270);
    } 

    return ESP_OK;
    
}

esp_err_t oled_draw_dot(const oled_dev_t* oled_dev, uint8_t x, uint8_t y){
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        s_white_buffer[i] = 0xFFFF;  // White pixel in RGB565
    }


    //ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x_start,y_start,x_start+2,y_start+2,s_dot_data));
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x,y,x+2,y+4,&s_dot_data));
    //ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x,y,x+2,y+4,&s_black_buffer));

    return ESP_OK;
}

esp_err_t oled_draw_letter(const oled_dev_t* oled_dev, uint8_t x, uint8_t y, uint8_t letter){
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x,y,x+8,y+8,&font[letter]));
    return ESP_OK;
}

esp_err_t oled_black_out_letter(const oled_dev_t* oled_dev, uint8_t x, uint8_t y){
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev->panel_handle,x,y,x+8,y+8,&font[0]));
    return ESP_OK;
}

//Helper Functions

static bool getBit(uint8_t byte, int pos)
{
    return (byte >> pos) & 1U;
}

static void setBit(uint8_t *byte, int pos, bool value)
{
    if (value) {
        *byte |= (1U << pos);
    } else {
        *byte &= ~(1U << pos);
    }
}

static void rotate8x8(uint8_t data[8], int rotation)
{
    // Create a temporary copy to hold the rotated bits
    uint8_t temp[8];
    
    // Normalize rotation to one of {0, 90, 180, 270}
    rotation = ((rotation % 360) + 360) % 360;

    // First, zero out temp
    for (int i = 0; i < 8; i++) {
        temp[i] = 0;
    }

    // We'll fill 'temp' with the rotated version of 'data'
    switch (rotation) {
        case 0:
            // No rotation -> just copy
            for (int row = 0; row < 8; row++) {
                temp[row] = data[row];
            }
            break;

        case 90:
            // new[r, c] = old[7 - c, r]
            for (int r_out = 0; r_out < 8; r_out++) {
                for (int c_out = 0; c_out < 8; c_out++) {
                    bool bitVal = getBit(data[7 - c_out], r_out);
                    setBit(&temp[r_out], c_out, bitVal);
                }
            }
            break;

        case 180:
            // new[r, c] = old[7 - r, 7 - c]
            for (int r_out = 0; r_out < 8; r_out++) {
                for (int c_out = 0; c_out < 8; c_out++) {
                    bool bitVal = getBit(data[7 - r_out], 7 - c_out);
                    setBit(&temp[r_out], c_out, bitVal);
                }
            }
            break;

        case 270:
            // new[r, c] = old[c_out, 7 - r_out]
            for (int r_out = 0; r_out < 8; r_out++) {
                for (int c_out = 0; c_out < 8; c_out++) {
                    bool bitVal = getBit(data[c_out], 7 - r_out);
                    setBit(&temp[r_out], c_out, bitVal);
                }
            }
            break;

        default:
            // Should never happen given the normalization above
            break;
    }

    // Now copy temp back into data
    for (int i = 0; i < 8; i++) {
        data[i] = temp[i];
    }
}

static void print8x8(const uint8_t data[8], const char *label)
{
    printf("%s\n", label);
    // We'll print bit 7 (left) down to 0 (right) for each row
    for (int row = 0; row < 8; row++) {
        for (int col = 7; col >= 0; col--) {
            bool bitVal = getBit(data[row], col);
            printf("%c", bitVal ? '1' : '0');
        }
        printf("\n");
    }
    printf("\n");
}

