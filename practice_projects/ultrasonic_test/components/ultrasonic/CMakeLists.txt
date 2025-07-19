#set(COMPONENT_SRCDIRS ".")
#set(COMPONENET_ADD_INCLUDEDIRS "include")
set(req driver freertos esp_timer)
idf_component_register(SRCS "ultrasonic.c"
                    INCLUDE_DIRS "include"
                    REQUIRES ${req})
