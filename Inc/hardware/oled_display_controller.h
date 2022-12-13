#ifndef STOPWATCH_OLEDDISPLAYCONTROLLER_H
#define STOPWATCH_OLEDDISPLAYCONTROLLER_H

#include <stdint.h>
#include <stdio.h>

#include "fonts.h"

#include "stm32f4xx_hal.h"

#define OLED_WIDTH           128
#define OLED_HEIGHT          64
#define OLED_I2C_PORT		 hi2c1

typedef enum {
    OLED_COLOR_BLACK = 0x00,
    OLED_COLOR_WHITE = 0x01
} OLED_COLOR;

void oledSetPixel(uint8_t x, uint8_t y, OLED_COLOR color);
void oledInit();
void oledUpdateScreen();
void oledFill(OLED_COLOR color);
void oledSetFont(Font *font);
uint8_t oledWriteChar(char ch, OLED_COLOR color);
uint8_t oledNextLine(uint8_t startX);
void oledSetCursor(uint8_t x, uint8_t y);
void oledSetLineHeight(float lineHeightRatio);
size_t oledWriteString(const char *str, OLED_COLOR color);

static const struct {
    void (*setPixel)(uint8_t x, uint8_t y, OLED_COLOR color);
    void (*init)();
    void (*updateScreen)();
    void (*fill)(OLED_COLOR color);
    void (*setFont)(Font *font);
    uint8_t (*writeChar)(char ch, OLED_COLOR color);
    uint8_t (*nextLine)(uint8_t startX);
    void (*setCursor)(uint8_t x, uint8_t y);
    void (*setLineHeight)(float lineHeightRatio);
    size_t (*writeString)(const char *str, OLED_COLOR color);
} Oled = {
        oledSetPixel,
        oledInit,
        oledUpdateScreen,
        oledFill,
        oledSetFont,
        oledWriteChar,
        oledNextLine,
        oledSetCursor,
        oledSetLineHeight,
        oledWriteString,
};

extern I2C_HandleTypeDef OLED_I2C_PORT;

#endif //STOPWATCH_OLEDDISPLAYCONTROLLER_H
