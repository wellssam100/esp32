//Include
#include <stdio.h>
#include <string.h>
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
#define I2C_SLAVE_ADDRESS 0x28
#define I2C_SLAVE_RX_BUF_LEN 128
#define I2C_SLAVE_TX_BUF_LEN 128
#define DATA_LENGTH 100

//Constants

static const char *TAG = "I2C_SLAVE";
static i2c_slave_dev_handle_t slave_dev_handle;

//Globals
//Need an event queue to take in events and process them
static QueueHandle_t s_receive_queue;
static uint8_t *temp_data;
static size_t temp_len = 0;

//Struct and Enums
typedef enum {
    I2C_SLAVE_EVT_RX,
    I2C_SLAVE_EVT_TX
} i2c_slave_event_t;

//Functions
//This function is used for the slave version 1 not 2
//static IRAM_ATTR bool i2c_slave_rx_done_callback(i2c_slave_dev_handle_t channel, const i2c_slave_rx_done_event_data_t *edata, void *user_data)
//{
//    ESP_LOGI(TAG, "Receive finishing...");
//    BaseType_t high_task_wakeup = pdFALSE;
//    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
//    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
//    return high_task_wakeup == pdTRUE;
//}

//Callback Functions
static bool i2c_slave_receive_cb(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_rx_done_event_data_t *evt_data, void *arg){
    //ESP_LOGI("Receiver", "Receiving Data");
    BaseType_t xTaskWoken;
    i2c_slave_event_t evt = I2C_SLAVE_EVT_RX;
    memcpy(temp_data, evt_data->buffer, evt_data->length);
    temp_len = evt_data->length;
    xQueueSendFromISR(s_receive_queue, &evt, &xTaskWoken);
    //ESP_L("Receiver", "Received Data");
    return xTaskWoken;
}

static bool i2c_slave_request_cb(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_request_event_data_t *evt_data, void *arg){
    ESP_LOGI("Example", "I am requesting a task");
    return true;
}

void app_main(void) {
    ESP_LOGI(TAG, "Initializing I2C slave...");
    //Create slave  config
    i2c_slave_config_t i2c_slave_config = {
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .send_buf_depth = DATA_LENGTH,
        .scl_io_num = I2C_SLAVE_SCL_IO,
        .sda_io_num = I2C_SLAVE_SDA_IO,
        .slave_addr = I2C_SLAVE_ADDRESS,
        .receive_buf_depth = DATA_LENGTH,
    };

    ESP_ERROR_CHECK(i2c_new_slave_device(&i2c_slave_config, &slave_dev_handle));

    ESP_LOGI(TAG, "I2C slave initialized!");

    //Create a queue to hand
    s_receive_queue = xQueueCreate(2, sizeof(i2c_slave_event_t));
    assert(s_receive_queue);
    temp_data = malloc(DATA_LENGTH);
    assert(temp_data);
    //Init Callbacks
    i2c_slave_event_callbacks_t cbs = {
            .on_receive = i2c_slave_receive_cb,
            .on_request = i2c_slave_request_cb,
    };
    ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(slave_dev_handle, &cbs, s_receive_queue));

    ESP_LOGI(TAG, "I2C slave callbacks initialized!");

    //Create test data
    uint8_t data_wr_test[DATA_LENGTH];
    for (int i = 0; i < DATA_LENGTH; i++) {
        data_wr_test[i] = i;
    }
    //Create receiving queue loop
    //First create an empty event to act as a placeholder for xQueueReceive
    i2c_slave_event_t evt;
    uint32_t write_len;
    while(true){
        //If there is an event in our queue of events. If xQueueReceive returns true
        //xQueueReceive is BLOCKING!!!
        if (xQueueReceive(s_receive_queue, &evt, 1) == pdTRUE){
            ESP_LOGI(TAG, "Received Event to queue, %d", evt);
            //If the event is a transmission event then we know that we should be writing
            if(evt == I2C_SLAVE_EVT_TX ){
                ESP_ERROR_CHECK(i2c_slave_write(slave_dev_handle, data_wr_test, DATA_LENGTH, &write_len, 1000));
            }
            //else if the event is a receive, then recieve
            else if(evt == I2C_SLAVE_EVT_RX){
                ESP_LOGI("Data Print", "Received Data");
                ESP_LOGI("Data Print", "%d", *temp_data);
            }
        }


    }
    free(temp_data);
    vQueueDelete(s_receive_queue);
    ESP_ERROR_CHECK(i2c_del_slave_device(slave_dev_handle));
}
