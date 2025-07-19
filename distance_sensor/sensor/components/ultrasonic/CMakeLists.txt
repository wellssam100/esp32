if(IDF_VERSION_MAJOR GREATER_EQUAL 5 AND IDF_VERSION_MINOR GREATER_EQUAL 1)
    set(gpio_dep esp_driver_gpio)
else()
    set(gpio_dep driver)
endif()

idf_component_register(SRCS "ultrasonic.c"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_driver_gpio freertos esp_timer)