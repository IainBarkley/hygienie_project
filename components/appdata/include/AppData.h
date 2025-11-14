#ifndef APPDATA_H
#define APPDATA_H
#include "AppDataConfig.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"
#include "stdlib.h"
#include <time.h>

struct Sanitizer_Data {
    bool pump_initialized;
    char device_name[32];
    char device_id[25];
    uint32_t max_offline_readings;
    uint32_t bootCount;
    uint32_t offlineReadingCount;
    time_t time_stamp_seconds[15];
    struct tm readings_temp[15];
};
#endif
