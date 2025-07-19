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
extern uint8_t num_font;
float dist=0;
static uint16_t s_white_buffer[WIDTH *HEIGHT] ;

//Sample Task
void ultrasonic_run(void *pvParameters){
    ultrasonic_t ultrasonic_dev = {17,5};
    ultrasonic_init(&ultrasonic_dev);
    uint32_t max_time = (uint32_t) pvParameters;
    ESP_LOGI(ULTRA_TAG, "%lu", max_time);
    dist = 0.0f;
    while(1){
        ultrasonic_measure_distance(&ultrasonic_dev, MAX_DISTANCE_M, &dist);
        //ESP_LOGI(ULTRA_TAG, "%.2f cm", dist);
        vTaskDelay(500/ portTICK_PERIOD_MS);//delay for one second
    }
}

void oled_run(void *pvParameters){
    oled_dev_t oled_dev = {0};
    i2c_master_bus_handle_t *bus_handle = (i2c_master_bus_handle_t *) pvParameters;
    oled_dev_init(&oled_dev, bus_handle, OLED_ADDR );
    ESP_LOGI("OLED MAIN", "test");

    static uint16_t s_black_buffer[WIDTH *HEIGHT] = {0};
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,0,0,WIDTH,HEIGHT,&s_black_buffer));
    int XCell = WIDTH/8;
    int YCell = HEIGHT/8;
    int i;
    int j;
    //Use a letter panel to draw on every spot
    for (i=0;i<XCell;i++){
        for (j=0;j<YCell;j++){
            if((j==(YCell/2-1)||j==YCell/2||j==(YCell/2+1))&&(i<=(XCell/2)+4&&i>=(XCell/2)-4)){
                continue;
            }
            ESP_ERROR_CHECK(oled_draw_letter(&oled_dev, i*8, j*8, 2));
        }
    }
    ESP_ERROR_CHECK(oled_draw_string(&oled_dev, (WIDTH/2)-32, HEIGHT/2,"SWELL LTD"));
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(oled_cover_string(&oled_dev, (WIDTH/2)-32, HEIGHT/2,"SWELL LTD"));


    //ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,0,0,WIDTH,HEIGHT,&s_black_buffer));
    
    //ESP_ERROR_CHECK(esp_lcd_panel_del(oled_dev.panel_handle));
    //ESP_ERROR_CHECK(esp_lcd_panel_io_del(oled_dev.i2c_handle));
    int x=1;
    char buffer[32];
    static uint8_t s_small_black_buffer[8] = {0};
    while(1){
        /**
        ESP_ERROR_CHECK(oled_draw_letter(&oled_dev, (WIDTH/2)-8, HEIGHT/2, x));
        ESP_ERROR_CHECK(oled_draw_letter(&oled_dev, (WIDTH/2)+8, HEIGHT/2, x));
        vTaskDelay(1000/ portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(oled_black_out_letter(&oled_dev, (WIDTH/2)-8, HEIGHT/2));
        ESP_ERROR_CHECK(oled_black_out_letter(&oled_dev, (WIDTH/2)+8, HEIGHT/2));
        x++;
        if(x==num_font){
            x=1;
        }
        */
        int buffLen = snprintf(buffer, sizeof(buffer), "%.1f", dist);
        ESP_ERROR_CHECK(oled_draw_string(&oled_dev, (WIDTH/2)-(8*(buffLen/2)), HEIGHT/2,buffer));
        vTaskDelay(500/ portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(oled_cover_string(&oled_dev, (WIDTH/2)-(8*(buffLen/2)), HEIGHT/2,buffer));

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

    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        s_white_buffer[i] = 0xFFFF;  // White pixel in RGB565
    }

    // pointer to task function, name for task (debug), stack size in bytes for task, parameters to pass to the task, task priority, handle to the task
    xTaskCreate(&ultrasonic_run, "Ultrasonic", 2048, max_time_p, 1, NULL);
    xTaskCreate(&oled_run, "OLED", 2048, &bus_handle, 2, NULL);

}