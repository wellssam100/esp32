/**
 * @file ultrasonic.h
 * @defgroup ultrasonic ultrasonic
 *  This library should allow user to activate the sensor and return the distance desired.
 * 
 */

// #include guard
#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__
//Needed to add gpio functionality in turning on and off the trigger pin 
#include <driver/gpio.h>
#include <esp_err.h>

//Error Definitions
#define ESP_ERR_ULTRASONIC_PING         0x200
#define ESP_ERR_ULTRASONIC_PING_TIMEOUT 0x201
#define ESP_ERR_ULTRASONIC_ECHO_TIMEOUT 0x202

/**
 * Device Descriptor
 * I don't think that I need any additional info besides signal listeners and senders
 */
typedef struct ultrasonic_t {
    gpio_num_t trigger_pin;
    gpio_num_t echo_pin;
} ultrasonic_t;

/**
 * 
 * @brief Setup GPIO pins of the ultrascale and set the trigger pin to low
 * @param dev Pointer to the device descriptor 
 * @return `ESP_OK` on success
 * 
 */
esp_err_t ultrasonic_init(const ultrasonic_t *dev);

/**
 * @brief Send an ultrasonic pulse and return the time between echo and receive
 * @param dev
 * @param max_wait fsdf
 * @param[out] time time variable in microseconds. This variable will be changed in the function running and won't have to be returned
 * 
 * @return `ESP_OK` on success, otherwise:
 *         - ::ESP_ERR_ULTRASONIC_PING         - Invalid state (previous ping is not ended)
 *         - ::ESP_ERR_ULTRASONIC_PING_TIMEOUT - Device is not responding
 *         - ::ESP_ERR_ULTRASONIC_ECHO_TIMEOUT - Distance is too big or wave is scattered
 */
esp_err_t ultrasonic_measure_time(const ultrasonic_t *dev, uint32_t max_wait, uint32_t *time);

/**
 * @brief Turn the time measurement into a distance measurement
 * @param max_dist should equal the distance of the max time
 * @param[out] dist Distance in meters
 * @return `ESP_OK` on success, otherwise:
 *         - ::ESP_ERR_ULTRASONIC_PING         - Invalid state (previous ping is not ended)
 *         - ::ESP_ERR_ULTRASONIC_PING_TIMEOUT - Device is not responding
 *         - ::ESP_ERR_ULTRASONIC_ECHO_TIMEOUT - Distance is too big or wave is scattered
 * 
 */
 esp_err_t ultrasonic_measure_distance(const ultrasonic_t *dev, uint32_t max_dist, uint32_t *dist);
 #endif

