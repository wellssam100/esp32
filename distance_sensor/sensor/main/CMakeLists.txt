
idf_component_register(SRCS "main.c" 
                       INCLUDE_DIRS "." 
                       PRIV_REQUIRES nvs_flash
                       REQUIRES esp_http_client esp_driver_i2c esp_wifi ultrasonic oled )