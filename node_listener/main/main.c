//Include

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c_slave.h" 
#include "esp_attr.h"

//Variable Definitions

#define I2C_SLAVE_SCL_IO 22               
#define I2C_SLAVE_SDA_IO 21               
#define I2C_SLAVE_ADDRESS 0x42
#define I2C_SLAVE_RX_BUF_LEN 128
#define I2C_SLAVE_TX_BUF_LEN 128
#define DATA_LENGTH 100

//Constants

static const char *TAG = "I2C_SLAVE";
static i2c_slave_dev_handle_t slave_dev_handle;


//Functions
static IRAM_ATTR bool i2c_slave_rx_done_callback(i2c_slave_dev_handle_t channel, const i2c_slave_rx_done_event_data_t *edata, void *user_data)
{
    ESP_LOGI(TAG, "Receive finishing...");
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}


void app_main(void) {
    ESP_LOGI(TAG, "Initializing I2C slave...");

    i2c_slave_config_t i2c_slave_config = {
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .send_buf_depth = 256,
        .scl_io_num = I2C_SLAVE_SCL_IO,
        .sda_io_num = I2C_SLAVE_SDA_IO,
        .slave_addr = I2C_SLAVE_ADDRESS,
    };

    ESP_ERROR_CHECK(i2c_new_slave_device(&i2c_slave_config, &slave_dev_handle));

    ESP_LOGI(TAG, "I2C slave initialized!");

    //Create a queue to hand
    QueueHandle_t s_receive_queue = xQueueCreate(1, sizeof(i2c_slave_rx_done_event_data_t));
    
    //Init Callbacks
    i2c_slave_event_callbacks_t cbs = {
        .on_recv_done = i2c_slave_rx_done_callback,
    };
    ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(slave_dev_handle, &cbs, s_receive_queue));

    uint8_t *data_rd = (uint8_t *) malloc(DATA_LENGTH);
    uint32_t size_rd = 0;

    i2c_slave_rx_done_event_data_t rx_data;
    ESP_ERROR_CHECK(i2c_slave_receive(slave_dev_handle, data_rd, DATA_LENGTH));
    xQueueReceive(s_receive_queue, &rx_data, pdMS_TO_TICKS(10000));
}
