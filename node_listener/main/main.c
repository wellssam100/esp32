#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// Listener Headers
#include "driver/i2c_slave.h"

i2c_slave_dev_handle_t setup_node_bus(){
    i2c_slave_dev_handle_t dev_handle;
    i2c_slave_config_t node_conf = {
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .send_buf_depth = 256,
        .scl_io_num = 22,
        .sda_io_num = 21,
        .slave_addr = 0x58,
    };

    i2c_slave_event_callbacks_t cbs = {
        .on_receive = i2c_slave_event_handler,
    };

    ESP_ERROR_CHECK(i2c_new_slave_device(&node_conf, &dev_handle));
    ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(dev_handle, i2c_slave_event_handler, NULL));
    ESP_LOGI("Slave", "I2C slave initialized with address 0x%X", 0x58);
    return dev_handle;
}

// Event handler for I2C slave
void i2c_slave_event_handler(i2c_slave_event_data_t *event_data, void *arg) {
    if (event_data->event_type == I2C_SLAVE_EVENT_RECV_DONE) {
        uint8_t data[BUFFER_SIZE];
        int read_len = i2c_slave_read_buffer(event_data->slave_handle, data, BUFFER_SIZE, 0);
        if (read_len > 0) {
            ESP_LOGI(TAG, "Received %d bytes: ", read_len);
            for (int i = 0; i < read_len; i++) {
                printf("%02X ", data[i]);
            }
            printf("\n");
        }
    }
}

void app_main(void){
    i2c_slave_dev_handle_t slave_handle=setup_node_bus();

    //i2c_slave_event_callbacks_t cbs = {
    //    .on_receive = i2c_slave_receive_cb,
    //    .on_request = i2c_slave_request_cb,
    //};

    //int data = 55;
    //ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(&slave_handle, &cbs, &data));

    while(1){
        printf(".");
        vTaskDelay(pdMS_TO_TICKS(1000));  // Periodic delay to keep the task alive
    }
}