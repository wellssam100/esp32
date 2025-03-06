/**
 * @file ultrasonic.c
 * 
 * Source adapted from https://github.com/ESP32Tutorials/HC-SR04-Ultrasonic-Sensor-with-ESP32-ESP-IDF
 */

 #include "include/ultrasonic.h"
 #include <esp_idf_version.h>
 #include <freertos/FreeRTOS.h>
 #include <freertos/task.h>
 #include <esp_timer.h>
 #include "esp_log.h"

 
 //Getting the build from the sdkconfig 
 // This needs to be specific to a project, not sure how to make this a library
 #if CONFIG_IDF_TARGET_ESP32
 #include <esp32/rom/ets_sys.h>
 static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
 #define PORT_ENTER_CRITICAL portENTER_CRITICAL(&mux)
 #define PORT_EXIT_CRITICAL portEXIT_CRITICAL(&mux)
 #else
 #error Target Build Incorrect
 #endif
 
 #define TRIGGER_LOW_DELAY 5
 #define TRIGGER_HIGH_DELAY 10
 #define PING_TIMEOUT 6000
 #define ROUNDTRIP 5800.0f
 #define CM_TO_INCH 0.393701
 #define SOUND_SPEED 0.034
 
 //if the current time is TIMEOUT time away from when the ping started, we have gone too long
 #define timeout_expired(start, len) ((esp_timer_get_time() - (start)) >= (len))
 
 esp_err_t ultrasonic_init(const ultrasonic_t *dev)
 {
     //Check that the dev is not null
     if (!(dev)){
         return ESP_ERR_INVALID_ARG;
     } 
     //Initialize pin and make sure each gpio function returns ESP_OK
     esp_err_t temp_err = gpio_set_direction(dev->trigger_pin, GPIO_MODE_OUTPUT);
     if (temp_err != ESP_OK){
         return temp_err;
     }
 
     temp_err = gpio_set_direction(dev->echo_pin, GPIO_MODE_INPUT);
     if (temp_err != ESP_OK){
         return temp_err;
     }
 
     //Turn off the trigger pin
     return gpio_set_level(dev->trigger_pin, 0);
 }
 
 esp_err_t ultrasonic_measure_time(const ultrasonic_t *dev, uint32_t max_wait, uint32_t *time){
     //
     if (!(dev)&&!(time)){
         return ESP_ERR_INVALID_ARG;
     }
     
     //Timing specific code block, stop interrupts
     portENTER_CRITICAL(&mux);
 
     esp_err_t temp_err = gpio_set_level(dev->trigger_pin,0);
     if(temp_err != ESP_OK){
         portEXIT_CRITICAL(&mux);
         return temp_err;
     }
     ets_delay_us(TRIGGER_LOW_DELAY);
 
     temp_err = gpio_set_level(dev->trigger_pin,1);
     if(temp_err != ESP_OK){
         portEXIT_CRITICAL(&mux);
         return temp_err;
     }
 
     ets_delay_us(TRIGGER_HIGH_DELAY);
 
     temp_err = gpio_set_level(dev->trigger_pin,0);
     if(temp_err != ESP_OK){
         portEXIT_CRITICAL(&mux);
         return temp_err;
     }
 
     //If the echo ping is still busy, we have a problem. Error out
     //We haven't started timing the echo yet
     if (gpio_get_level(dev->echo_pin)){
         portEXIT_CRITICAL(&mux);
         return ESP_ERR_ULTRASONIC_PING;
     }
 
     //Start timing and wait to hear from the echo
     int64_t start = esp_timer_get_time();
     while (!gpio_get_level(dev->echo_pin)){
         if(timeout_expired(start, PING_TIMEOUT)){
             portEXIT_CRITICAL(&mux);
             return ESP_ERR_ULTRASONIC_PING_TIMEOUT;
         }
     }
 
     //Received echo, measure
     int64_t echo_receive = esp_timer_get_time();
     int64_t temp_time = echo_receive;
     while (gpio_get_level(dev->echo_pin)){
         temp_time = esp_timer_get_time();
         if (timeout_expired(echo_receive, max_wait)){
             portEXIT_CRITICAL(&mux);
             return ESP_ERR_ULTRASONIC_PING_TIMEOUT;
         }
     }
     portEXIT_CRITICAL(&mux);
 
     *time = temp_time - echo_receive;
 
     return ESP_OK;
 }
 
 esp_err_t ultrasonic_measure_distance(const ultrasonic_t *dev, uint32_t max_dist, uint32_t *dist)
 {
     if(!(dev)&&!(dist)){
         return ESP_ERR_INVALID_ARG;
     }
     uint32_t max_time = max_dist * ROUNDTRIP; 
     uint32_t time;
     esp_err_t temp_err = ultrasonic_measure_time(dev, max_time, &time);
     if(temp_err != ESP_OK){
         return temp_err;
     }
     *dist = time * 0.034/2;
     return ESP_OK;
 }