#Tells ESP-IDF how to build the project
cmake_minimum_required(VERSION 3.16)

#Points to the CMake files provided by ESP-IDF. This allows child CMake files to use ESP-IDF marcos
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
#Name the Project
project(ultrasonic_device)