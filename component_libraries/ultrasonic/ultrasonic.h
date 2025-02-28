// #include guard
#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__



//Getting the build from the sdkconfig 
// This needs to be specific to a project, not sure how to make this a library
#if CONFIG_IDF_TARGET_ESP32
#include <esp32/rom/ets_sys.h>
/*
#elif CONFIG_IDF_TARGET_ESP32S2
#include <esp32s2/rom/ets_sys.h>
#elif CONFIG_IDF_TARGET_ESP32S3
#include <esp32s3/rom/ets_sys.h>
#elif CONFIG_IDF_TARGET_ESP32C3
#include <esp32c3/rom/ets_sys.h>
#elif CONFIG_IDF_TARGET_ESP32H2
#include <esp32h2/rom/ets_sys.h>
#elif CONFIG_IDF_TARGET_ESP32C2
#include <esp32c2/rom/ets_sys.h>
*/
#endif


#include <freertos/FreeRTOS.h>
#include <esp_idf_version.h>

#include <driver/gpio.h>
#include <esp_err.h>