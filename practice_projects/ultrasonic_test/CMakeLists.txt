#Tells ESP-IDF how to build the project
cmake_minimum_required(VERSION 3.16)

#Add component dirs that aren't under the folder 'components'
#set(EXTRA_COMPONENT_DIRS "C:/Users/wells/Workspace/ESP32/projects/component_libraries/")
#set(COMPONENTS main ultrasonic)
set(COMPONENTS main ultrasonic esp_lcd esp_driver_i2c oled)
#Points to the CMake files provided by ESP-IDF. This allows child CMake files to use ESP-IDF marcos
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
#Name the Project
project(ultrasonic)