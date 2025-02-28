//Includes
//  General includes
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_err.h"

//  I2C includes
#include "driver/i2c_master.h"

//  LCD includes
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

//  UART includes
#include "driver/uart.h"

// Variables
//  I2C Vars
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_FREQ_HZ 100000
#define I2C_MASTER_TIMEOUT_MS 1000
#define I2C_SLAVE_ADDRESS 0x28   
//  LCD Vars
#define LCD_I2C_DEV_ADDR 0x3C
#define LCD_PIXEL_CLOCK_HZ (400*1000)
//  UART Vars
#define UART_NUM UART_NUM_0 


//Constants
static const char *TAG = "Master_Menu";

//  I2C master consts
static i2c_master_bus_handle_t master_bus_handle;
static i2c_master_dev_handle_t master_dev_handle;

//  LCD consts
static esp_lcd_panel_io_handle_t lcd_io_handle;

static esp_lcd_panel_handle_t panel_handle=NULL;
static esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = -1,
};

//  UART consts

//Structs
struct command{
    char* command;
    bool ISRSafe;
    int cmdId;
};

//Global Variables
//  Mutex Variables
SemaphoreHandle_t xPrintMutex;
struct command command_array[];



//Functions
//  LCD Setup Function
bool setup_i2c_devs(){
    ESP_LOGI(TAG, "Setting up Devices");    
    //Master Bus
     i2c_master_bus_config_t bus_config = {
        .sda_io_num = I2C_MASTER_SDA_IO,       
        .scl_io_num = I2C_MASTER_SCL_IO,      
        .flags={
            .enable_internal_pullup = true
        },
        .clk_source = I2C_CLK_SRC_DEFAULT,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &master_bus_handle));

    //Master Device
    i2c_device_config_t master_device_config = {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = I2C_SLAVE_ADDRESS,
        .scl_speed_hz = 400000, 
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(master_bus_handle, &master_device_config, &master_dev_handle));
    
    //LCD I2C device
    esp_lcd_panel_io_i2c_config_t lcd_io_config = {
        .dev_addr = LCD_I2C_DEV_ADDR,
        .scl_speed_hz = LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1, // According to SSD1306 datasheet
        .dc_bit_offset = 6,       // According to SSD1306 datasheet
        .lcd_cmd_bits = 8,        // According to SSD1306 datasheet
        .lcd_param_bits = 8,      // According to SSD1306 datasheet
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(master_bus_handle, &lcd_io_config, &lcd_io_handle));
    //LCD Panel
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(lcd_io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, false));
    return true;
}

bool draw_lcd(){
    ESP_LOGI(TAG, "Drawing");    
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    //const uint16_t temp_data[8] = {0x1001,0x1001,0x0001,0x0001,0x0001,0x0001,0x0001,0x1001};
    const uint8_t temp_data[8] = {0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81};
    //const uint8_t temp_data[16] = {0x81,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0x81};
    const uint8_t temp_data_2[16] = {0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90,0xA0,0xB0,0xC0,0xD0,0xE0,0xF0};

    //const void *int_ptr = &temp_data;
    const uint8_t pattern[][16] = {{
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        }};
    //    {
    //        0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81,
    //        0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81
    //    },
    //    //A and B
    //    {
    //        0x00, 0x3C, 0x66, 0x42, 0x7E, 0x42, 0x42, 0x00,
    //        0x00, 0x7E, 0x52, 0x52, 0x52, 0x54, 0x2C, 0x00
    //    }
    //};
    
    for (int i = 0; i < 128 / 16; i++) {
        for (int j = 0; j < 64 / 8; j++) {
            ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, i * 16, j * 8, i * 16 + 16, j * 8 + 8, pattern[0]));
        }
    }
    //ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle,0,0,8,8,temp_data));
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle,64,8,80,16,temp_data));
    
    return true;
}

bool cleanup(){
    ESP_LOGI(TAG, "Cleanup");    
    ESP_ERROR_CHECK(esp_lcd_panel_del(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_io_del(lcd_io_handle));
    ESP_ERROR_CHECK(i2c_del_master_bus(master_bus_handle));
    return true;
}

bool show_menu(){
    xSemaphoreTake(xPrintMutex, portMAX_DELAY);
    printf("| %-10s|", "Menu");
    xSemaphoreGive(xPrintMutex);

    return true;
}

bool handle_menu(char input){
    xSemaphoreTake(xPrintMutex, portMAX_DELAY);
    switch (input){
        case '1':
            printf("you chose 1");
            break;
        case '2':
            printf("you chose 2");
            break;
        default:
            printf("You picked something I haven't made yet");
    }
    xSemaphoreGive(xPrintMutex);
    return true;
}

void serial_menu_task(void *pvParameter){
    uint8_t data[1024];
    while (1){
        show_menu();
        int len = uart_read_bytes(UART_NUM, data, 1024 - 1, pdMS_TO_TICKS(50000));
        if(len>0){
            data[len] = '\0';
            ESP_LOGI(TAG, "User input: %s", (char *)data);
            handle_menu(data[0]);
        }
        else{
            ESP_LOGI(TAG, "UART Read Timed-out");
        }
    }
}


//Sample Task
void sample_task(void *pvParameter){
    while(1){
        printf("Hello World\n");
        vTaskDelay(1000/ portTICK_PERIOD_MS);//delay for one second
    }
}



void app_main(void) {
    ESP_LOGI(TAG, "Starting");   
    xPrintMutex = xSemaphoreCreateMutex();
    if(xPrintMutex == NULL){
        ESP_LOGI(TAG, "Failed to create mutex\n");
        return;
    } 
    setup_i2c_devs();
    xTaskCreate(&serial_menu_task, "Menu", 2048, NULL, 1, NULL);
    draw_lcd();
    cleanup();

    // pointer to task function, name for task (debug), stack size in bytes for task, parameters to pass to the task, task priority, handle to the task
    //xTaskCreate(&sample_task, "Sample", 2048, NULL, 1, NULL);

    //Setup UART

    //Setup lcd

    //Setup Master

    //Start Menu Task

}