//Skkeleton main.c file

#include <stdio.h>
//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//My Components
#include "../components/ultrasonic/include/ultrasonic.h"
#include "../components/oled/include/oled.h"
//General ESP Use
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c_master.h"
#include "esp_random.h"

//Ultrasonic Consts
static const char *ULTRA_TAG = "ULTRASONIC_TEST";
#define MAX_DISTANCE_M 500 // 5m max
#define TRIGGER_GPIO 5
#define ECHO_GPIO 18

//I2C Consts
#define I2C_MASTER_SDA 21
#define I2C_MASTER_SCL 22

//OLED Consts
#define OLED_ADDR 0x3C
#define HEIGHT 64
#define WIDTH 128

//Sample Task
void ultrasonic_run(void *pvParameters){
    ultrasonic_t ultrasonic_dev = {17,5};
    ultrasonic_init(&ultrasonic_dev);
    uint32_t max_time = (uint32_t) pvParameters;
    ESP_LOGI(ULTRA_TAG, "%lu", max_time);
    float dist = 0.0f;
    while(1){
        ultrasonic_measure_distance(&ultrasonic_dev, MAX_DISTANCE_M, &dist);
        ESP_LOGI(ULTRA_TAG, "%.2f cm", dist);
        vTaskDelay(1000/ portTICK_PERIOD_MS);//delay for one second
    }
}

void oled_run(void *pvParameters){
    oled_dev_t oled_dev = {0};
    i2c_master_bus_handle_t *bus_handle = (i2c_master_bus_handle_t *) pvParameters;
    oled_dev_init(&oled_dev, bus_handle, OLED_ADDR );
    ESP_LOGI("OLED MAIN", "test");
    
    
    static uint16_t s_white_buffer[WIDTH *HEIGHT] ;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        s_white_buffer[i] = 0xFFFF;  // White pixel in RGB565
    }
    static uint16_t s_black_buffer[WIDTH *HEIGHT] = {0};
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,0,0,WIDTH,HEIGHT,&s_black_buffer));
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,0,0,WIDTH,HEIGHT,&s_white_buffer));
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,0,0,WIDTH,HEIGHT,&s_black_buffer));
    ESP_LOGI("OLED MAIN", "black out");
    //ESP_ERROR_CHECK(esp_lcd_panel_del(oled_dev.panel_handle));
    //ESP_ERROR_CHECK(esp_lcd_panel_io_del(oled_dev.i2c_handle));
    int x=0;
    int y=10;
    static uint16_t s_small_black_buffer[2] = {0};

    while(1){
        while(x<WIDTH-2){
            //ESP_LOGI("OLED MAIN", "draw dot, %d", x);
            oled_draw_dot(&oled_dev,x,y);
            vTaskDelay(10/ portTICK_PERIOD_MS);
            x++;
        }
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,x-1,y,x+3,y+4,&s_small_black_buffer));
        vTaskDelay(100/ portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,x-2,y,x+2,y+4,&s_small_black_buffer));
        y= esp_random() % 63;
        x=0;
        //while(y<HEIGHT){
        //    ESP_LOGI("OLED MAIN", "draw dot, %d", x);
        //    oled_draw_dot(&oled_dev,10,y);
        //    vTaskDelay(500/ portTICK_PERIOD_MS);
        //    y++;
        //}

        //ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,0,0,128,64,&s_black_buffer));
        //vTaskDelay(2000/ portTICK_PERIOD_MS);
        //ESP_LOGI("OLED MAIN", "white");
        //ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,0,0,128,64,&s_white_buffer));
        //vTaskDelay(2000/ portTICK_PERIOD_MS);
    }

}

void app_main(void) {
    printf("Hello World\n");
    uint32_t max_time = 10;
    void * max_time_p = &max_time;
    //Init master i2c controller
    i2c_master_bus_config_t i2c_bus_conf = { 
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .sda_io_num = I2C_MASTER_SDA,
        .scl_io_num = I2C_MASTER_SCL,
        .i2c_port = -1,
    };
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_conf, &bus_handle));


    // pointer to task function, name for task (debug), stack size in bytes for task, parameters to pass to the task, task priority, handle to the task
    xTaskCreate(&ultrasonic_run, "Ultrasonic", 2048, max_time_p, 1, NULL);
    xTaskCreate(&oled_run, "OLED", 2048, &bus_handle, 2, NULL);

}