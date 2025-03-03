//Skkeleton main.c file

#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "component_libraries/ultrasonic"

//Sample Task
void sample_task(void *pvParameter){
    while(1){
        printf("Hello World\n");
        vTaskDelay(1000/ portTICK_PERIOD_MS);//delay for one second
    }
}

void app_main(void) {
    printf("Hello World\n");
    // pointer to task function, name for task (debug), stack size in bytes for task, parameters to pass to the task, task priority, handle to the task
    xTaskCreate(&sample_task, "Sample", 2048, NULL, 1, NULL);

}