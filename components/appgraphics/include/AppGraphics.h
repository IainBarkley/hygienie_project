#ifndef APPGRAPHICS_H
#define APPGRAPHICS_H
#include "esp_log.h"
#include "u8g2_esp32_hal.h"
#include <driver/gpio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <time.h>
#include <u8g2.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* SDA - GPIO21 */
#define PIN_SDA 21

/* SCL - GPIO22 */
#define PIN_SCL 22

/* RST - RST_PIN */
#define RST_PIN 25

#define LCD_ENABLE_PIN 13

#define GRAPHICS_DELAY_MS CONFIG_GRAPHICS_DELAY_MS

/* Different graphic images */
#define NETWORK_ICON    0X0119
#define UPLOAD_ICON     0x008F
#define CHECKMARK_ICON  0x0073
#define STAR_ICON       0x0102
#define DISPLAY_ADDRESS 0x0078
#define TEAR_DROP       0x0098

/* Box of display boundaries */
#define X_UPPER_LEFT  20
#define Y_UPPER_LEFT  80
#define WIDTH_OF_BOX  20
#define HEIGHT_OF_BOX 30

/* Glyph coordinates */
#define GLPYH_X_COORDINATE 10
#define GLYPH_Y_COORDINATE 60

/* Str coordinates */
#define STRING_X_COORDINATE 30
#define STRING_Y_COORDINATE 60

/* Address within u8g2 graphics library */
#define START_ADDRESS_OF_ANIMALS 0x0030

void AppGraphicsInitDisplay(void);

void AppGraphicsAnimationCycle(void);

void AppGraphicsPrintDroplet(void);

void AppGraphicsPrintOnOled(int32_t text, int32_t height);

void AppGraphicsHandleText(uint32_t text);

void AppGraphicsHandleGraphics(uint32_t text);

void AppGraphicsWakeUpDisplay(void);

void AppGraphicsCloseDisplay(void);

void AppGraphicsClearBuffer(void);
#endif
