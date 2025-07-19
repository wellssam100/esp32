#set(COMPONENT_SRCDIRS ".")
#set(COMPONENET_ADD_INCLUDEDIRS "include")
set(req driver freertos)
idf_component_register(SRCS "oled.c"
                       PRIV_REQUIRES unity esp_driver_i2c
                       WHOLE_ARCHIVE
                       REQUIRES esp_lcd)

