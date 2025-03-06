//Skkeleton main.c file

#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../components/ultrasonic/include/ultrasonic.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "ULTRASONIC_TEST";
#define MAX_DISTANCE_M 500 // 5m max
#define TRIGGER_GPIO 5
#define ECHO_GPIO 18


//Sample Task
void sample_task(void *pvParameters){
    ultrasonic_t ultrasonic_dev = {17,5};
    ultrasonic_init(&ultrasonic_dev);
    uint32_t max_time = (uint32_t) pvParameters;
    ESP_LOGI(TAG, "%lu", max_time);
    uint32_t dist = 0;
    while(1){
        ultrasonic_measure_distance(&ultrasonic_dev, MAX_DISTANCE_M, &dist);
        ESP_LOGI(TAG, "%lu cm", dist);
        vTaskDelay(1000/ portTICK_PERIOD_MS);//delay for one second
    }
}

void app_main(void) {
    printf("Hello World\n");
    uint32_t max_time = 10;
    void * max_time_p = &max_time;
    // pointer to task function, name for task (debug), stack size in bytes for task, parameters to pass to the task, task priority, handle to the task
    xTaskCreate(&sample_task, "Sample", 2048, max_time_p, 1, NULL);

}