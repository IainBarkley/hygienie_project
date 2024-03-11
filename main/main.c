#include <stdio.h>
#include <stdlib.h>
#include "esp_system.h"
#include "testable.h"
#include "esp_wifi.h"
#include "driver/adc.h"
#include "AppGraphics.h"
#include "appmqtt.h"
#include "AppSleep.h"
#include "appwifi.h"

static const char *TAG = "App Main";

void app_main() {

    esp_sleep_wakeup_cause_t wakeupCause = AppSleepGetWakeUpCause();
    AppMqttAddTime();
    switch (wakeupCause) {

        case ESP_SLEEP_WAKEUP_TIMER:
            AppSleepLog();
            if (AppMqttGetNumoffLineReadingCount() > 0 ) {
                AppWifiStart();
                AppMqttSendData();
                AppWifiDisconnect();
            }
            break;

        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            ESP_LOGI(TAG, "Wake up from touch on pad %d\n", esp_sleep_get_touchpad_wakeup_status());
            AppGraphicsAnimationCycle();
            if (AppMqttGetNumoffLineReadingCount() >= MAX_OFFLINE_READINGS-1) {
                AppWifiStart();
                AppMqttSendData();
                AppWifiDisconnect();
            }
            break;

        case ESP_SLEEP_WAKEUP_UNDEFINED:
            ESP_LOGI(TAG, "Not a deep sleep or a touch wake_up\n");
            break;
        default:
            ESP_LOGI(TAG, "ERROR with wake up cause\n");
    }
    AppSleepInit();
    AppSleepGoToDeepSleep();
}

