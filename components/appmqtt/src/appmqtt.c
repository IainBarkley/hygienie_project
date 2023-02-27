#include "appmqtt.h"
#include <string.h>

static RTC_DATA_ATTR struct Sanitizer_Data sanitizer_data = { .pump_initialized = false,
                                                              .PUMP_ID = "test_id",
                                                              .max_offline_readings = MAX_OFFLINE_READINGS,
                                                              .bootCount = 10,
                                                              .offlineReadingCount = 0};

static esp_mqtt_client_config_t mqtt_cfg = {
.uri = "mqtts://localhost:8883",
//.event_handle = mqtt_event_handler,
};


static esp_mqtt_client_handle_t client;

//static double read_battery_level() {
//    rawValue = 0;
//    double voltageLevel = (rawValue / 4095.0) * 2 * 1.1 * 3.3; // calculate voltage level
//    double batteryFraction = ( voltageLevel - MIN_BATTERY_VOLTAGE ) / ( MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE );
//
//    return batteryFraction;
//
//}

//static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
//{
//    esp_mqtt_client_handle_t client = event->client;
//    switch (event->event_id) {
//        case MQTT_EVENT_CONNECTED:
//            // Handle connection event
//            break;
//        case MQTT_EVENT_DISCONNECTED:
//            // Handle disconnection event
//            break;
//        case MQTT_EVENT_PUBLISHED:
//            // Handle publish event
//            break;
//        // Add other event handlers as needed
//    }
//    return ESP_OK;
//}

void AppMqttInit() {
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
    return;
}

void AppMqttDestroy() {
    esp_mqtt_client_destroy(client);
    return;
}

void AppMqttCreateJson() {

    cJSON *root =cJSON_CreateObject();
//    double batteryFraction = read_battery_level();
 //   cJSON_AddNumberToObject(root, "Battery_Fraction", batteryFraction);
    cJSON_AddStringToObject(root, "FacilityName", facility_name);
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "uses", sanitizer_data.readings_temp);
    char *json_str = cJSON_Print(root);
    AppMqttPublish(json_str);
    AppMqttDestroyJson(root);
    free(json_str);
    return;
 }

void AppMqttAddTime() {
    //time( &raw_time );
    time_t now;
    //struct tm timeinfo;
    char strftime_buf[64];
    time(&now);
// Set timezone to China Standard Time
    //setenv("TZ", "MT", 1);
    //tzset();
    //localtime_r(&now, &timeinfo);
    //strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
   //printf("The current date/time in Calgary is: %s", strftime_buf);


    struct tm * timeinfo = localtime( &now );
    sanitizer_data.readings_temp[sanitizer_data.offlineReadingCount] = timeinfo;
    sanitizer_data.offlineReadingCount++;
    return;
}

int AppMqttGetNumoffLineReadingCount() {
    int off_line_count = sanitizer_data.offlineReadingCount;
    return off_line_count;
}

void AppMqttDestroyJson(cJSON *root) {
    cJSON_Delete(root);
    return;
}

void AppMqttPublish(char *json_str) {
    esp_mqtt_client_publish(client, "example/topic", json_str, strlen(json_str), 0, 0 );
    return;
}

void AppMqttSetOffineReadingCount(int offlineReadingCount) {
    sanitizer_data.offlineReadingCount = offlineReadingCount;
    return;
}

void AppMqttSendData(void) {
    AppMqttInit();
    AppMqttCreateJson();
    AppMqttDestroy();
}

