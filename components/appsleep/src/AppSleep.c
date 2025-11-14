#include "AppSleep.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define GPIO_WAKEUP_PIN 27

static const char *TAG = "AppSleep";

void AppSleepInit(void)
{
    AppSleepDeepSleepTimerInit();
    AppSleepWakeUpInit();
}

void AppSleepGoToDeepSleep(void)
{
    esp_deep_sleep_start();
}

esp_sleep_wakeup_cause_t AppSleepGetWakeUpCause(void)
{
    return esp_sleep_get_wakeup_cause();
}

void AppSleepDeepSleepTimerInit(void)
{
    uint32_t wakeup_time_sec;
#ifdef DEEP_SLEEP_TIMER
    wakeup_time_sec = DEEP_SLEEP_TIMER * 60;
#else
    /* Default to 10 minutes if DEEP_SLEEP_TIMER is not defined */
    wakeup_time_sec = 600;
#endif
    esp_sleep_enable_timer_wakeup(wakeup_time_sec * 1000000);
}

void AppSleepConfigureGpioForSleep(void)
{

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << GPIO_WAKEUP_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    int gpio_level = gpio_get_level(GPIO_WAKEUP_PIN);
    ESP_LOGI(TAG, "GPIO 27 level before sleep: %d (should be 1 with pull-up)", gpio_level);
}

void AppSleepWakeUpInit()
{
    /* Configure GPIO 27 as an external wakeup pin */
    esp_sleep_enable_ext0_wakeup(GPIO_WAKEUP_PIN, 0);

    /* Configure power domain to keep RTC peripherals on during deep sleep */
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

    /* Verify wake-up is configured */
    ESP_LOGI(TAG, "About to enter deep sleep - EXT0 wake-up should be configured");
}
