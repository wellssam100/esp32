#Tells ESP-IDF how to build the project
cmake_minimum_required(VERSION 3.16)
#set(COMPONENTS main ultrasonic esp_lcd esp_driver_i2c oled esp_http_client esp_wifi)

#Points to the CMake files provided by ESP-IDF. This allows child CMake files to use ESP-IDF marcos
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
#Name the Project
project(sensor)