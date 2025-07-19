//Sensor; collects dist data, prints to oled screen and transmits to webserver
//INCLUDES
#include <stdio.h>
//FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//I2C
#include "driver/i2c_master.h"
//HTTP
#include "esp_http_client.h"
//Wifi
#include "esp_wifi.h"
//NVS
#include "nvs_flash.h"
//Ultrasonic
#include "../components/ultrasonic/include/ultrasonic.h"
//Oled
#include "../components/oled/include/oled.h"
//General Use
#include "esp_log.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_random.h"


//CONSTANTS
//Ultrasonic
#define TRIGGER_GPIO 5
#define ECHO_GPIO 17
#define MAX_DISTANCE_M 500// 5 meter max
//I2C
#define I2C_MASTER_SDA 21
#define I2C_MASTER_SCL 22
//OLED
#define OLED_ADDR 0x3C
#define HEIGHT 64
#define WIDTH 128

//Wifi
//This just gets the configurations of the wifi in line with the configs of menuconfig
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
#define EXAMPLE_H2E_IDENTIFIER ""
// My Wifi
#define SERVER_SSID "esp32_ssid"
#define SERVER_PWD "esp32_pwd"
// Wifi Event Handler Setup
/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define EXAMPLE_ESP_MAXIMUM_RETRY 3

//Event Group to receive events from the default FreeRTOS event group
static EventGroupHandle_t s_wifi_event_group;

float dist = 0.0f;
static int s_retry_num = 0;

static const char *TAG = "SENSOR";

//FUNCTIONS
//Setup http_client
//Post Value
void post_sensor_value(const float *dist_f){
    char post_data[64];

    snprintf(post_data, sizeof(post_data), "value=%.1f", *dist_f);

    esp_http_client_config_t config = {
        .url = "http://192.168.4.1/update",  // <-- IP of the webserver ESP
        .method = HTTP_METHOD_POST,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI("HTTP", "POST success, status = %d", esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE("HTTP", "POST failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
    ESP_LOGI("HTTP", "Sending POST to %s", config.url);
    ESP_LOGI("HTTP", "Header: Content-Type: application/x-www-form-urlencoded");
    ESP_LOGI("HTTP", "Post Body: %s", post_data);

}
//Ultrasonic Task
void ultrasonic_run(void *pvParameters){
    ultrasonic_t ultrasonic_dev = {17,5};
    ultrasonic_init(&ultrasonic_dev);
    uint32_t max_time = 10;
    //float dist = 0.0f;
    ESP_LOGI(TAG, "%lu", max_time);
    while(1){
        ESP_ERROR_CHECK(ultrasonic_measure_distance(&ultrasonic_dev, MAX_DISTANCE_M, &dist));
        //SEND DATA HERE
        post_sensor_value(&dist);
        vTaskDelay(500/ portTICK_PERIOD_MS);
    }
}

//Oled Task
//Setup Oled

void oled_run(void *pvParameters){
    oled_dev_t oled_dev = {0};
    i2c_master_bus_handle_t *bus_handle = (i2c_master_bus_handle_t *) pvParameters;
    oled_dev_init(&oled_dev, bus_handle, OLED_ADDR );
    ESP_LOGI("OLED MAIN", "test");

    static uint16_t s_black_buffer[WIDTH *HEIGHT] = {0};
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(oled_dev.panel_handle,0,0,WIDTH,HEIGHT,&s_black_buffer));
    int XCell = WIDTH/8;
    int YCell = HEIGHT/8;
    int i;
    int j;
    //Use a letter panel to draw on every spot
    for (i=0;i<XCell;i++){
        for (j=0;j<YCell;j++){
            if((j==(YCell/2-1)||j==YCell/2||j==(YCell/2+1))&&(i<=(XCell/2)+4&&i>=(XCell/2)-4)){
                continue;
            }
            ESP_ERROR_CHECK(oled_draw_letter(&oled_dev, i*8, j*8, 2));
        }
    }
    ESP_ERROR_CHECK(oled_draw_string(&oled_dev, (WIDTH/2)-32, HEIGHT/2,"SWELL LTD"));
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(oled_cover_string(&oled_dev, (WIDTH/2)-32, HEIGHT/2,"SWELL LTD"));

    int x=1;
    char buffer[32];
    static uint8_t s_small_black_buffer[8] = {0};
    while(1){
        int buffLen = snprintf(buffer, sizeof(buffer), "%.1f", dist);
        ESP_ERROR_CHECK(oled_draw_string(&oled_dev, (WIDTH/2)-(8*(buffLen/2)), HEIGHT/2,buffer));
        vTaskDelay(500/ portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(oled_cover_string(&oled_dev, (WIDTH/2)-(8*(buffLen/2)), HEIGHT/2,buffer));

    }

}

//Wifi event Handler
static void wifi_event_handler(void* arg, esp_event_base_t event_base, 
                                int32_t event_id, void* event_data){
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}
//Setup wifi
void wifi_init(){

    s_wifi_event_group = xEventGroupCreate();
    //Setup Network Interface
    // 1) Initialize the TCP/IP stack and the event loop
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // Creates the network interface handle, driver, glue, registers ESP-NETIF events, attaches the driver's glue layer to the interface handle
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

    //Register Event Handlers
    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = SERVER_SSID,
            .password = SERVER_PWD,
            .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
            .sae_pwe_h2e = ESP_WIFI_SAE_MODE,
            .sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);
    //Test
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
            SERVER_SSID, SERVER_PWD);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
            SERVER_SSID, SERVER_PWD);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}



void app_main(void) {
    //Init master i2c controller
    i2c_master_bus_config_t i2c_bus_conf = { 
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .sda_io_num = I2C_MASTER_SDA,
        .scl_io_num = I2C_MASTER_SCL,
        .i2c_port = -1,
    };
    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_conf, &bus_handle));

    
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    vTaskDelay(pdMS_TO_TICKS(1000));
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    esp_err_t err = esp_reset_reason();
    ESP_LOGI("RESET", "Reset reason: %d", err);

    wifi_init();
    xTaskCreate(&oled_run, "OLED", 4096, &bus_handle, 2, NULL);
    vTaskDelay(pdMS_TO_TICKS(1000));
    xTaskCreate(&ultrasonic_run, "Ultrasonic", 4096, NULL, 1, NULL);
}