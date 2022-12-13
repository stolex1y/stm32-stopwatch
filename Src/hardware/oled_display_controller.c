#include <hardware/oled_display_controller.h>
#include <stdlib.h>

#include "fonts.h"
#include "i2c.h"

#define OLED_I2C_ADDR        0x78
#define LINES_PER_PAGE        8

#define bitSet(byte, n) ((byte) | (1 << (n)))
#define bitReset(byte, n) ((byte) & (~(1 << (n))))

struct Oled {
    uint8_t x;
    uint8_t y;
    Font *font;
    uint8_t lineHeight;
} oled = {
        .x = 0,
        .y = 0,
        .font = NULL,
        .lineHeight = 0
};

static uint8_t buffer[OLED_HEIGHT / LINES_PER_PAGE][OLED_WIDTH];

static void oledWriteCommand(uint8_t command);

void oledInit() {
    HAL_Delay(100);

    oledWriteCommand(0xAE);

    oledWriteCommand(0x20); // set page addressing mode
    oledWriteCommand(0x10);

    oledWriteCommand(0xB0); // set page start: 0
    oledWriteCommand(0xC8);

    oledWriteCommand(0x00); // set column start address: 0x00
    oledWriteCommand(0x10);

    oledWriteCommand(0x40);
    oledWriteCommand(0x81);
    oledWriteCommand(0xFF);
    oledWriteCommand(0xA1);
    oledWriteCommand(0xA6);
    oledWriteCommand(0xA8);
    oledWriteCommand(0x3F);
    oledWriteCommand(0xA4);
    oledWriteCommand(0xD3);
    oledWriteCommand(0x00);
    oledWriteCommand(0xD5);
    oledWriteCommand(0xF0);
    oledWriteCommand(0xD9);
    oledWriteCommand(0x22);
    oledWriteCommand(0xDA);
    oledWriteCommand(0x12);
    oledWriteCommand(0xDB);
    oledWriteCommand(0x20);
    oledWriteCommand(0x8D);
    oledWriteCommand(0x14);
    oledWriteCommand(0xAF);

    oledFill(OLED_COLOR_BLACK);
    oledUpdateScreen();
    oled.font = &font11x18;
    oled.lineHeight = (oled.font)->height + 5;
}

void oledFill(OLED_COLOR color) {
    for (uint8_t x = 0; x < OLED_WIDTH; x++) {
        for (uint8_t y = 0; y < OLED_HEIGHT; y++) {
            oledSetPixel(x, y, color);
        }
    }
}

void oledSetPixel(uint8_t x, uint8_t y, OLED_COLOR color) {
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT)
        return;

    uint8_t page = y / LINES_PER_PAGE;
    uint8_t *column = buffer[page] + x;
    if (color == OLED_COLOR_WHITE) {
        *column = bitSet(*column, y % 8);
    } else {
        *column = bitReset(*column, y % 8);
    }
}

void oledUpdateScreen() {
    uint8_t i;

    for (i = 0; i < 8; i++) {
        oledWriteCommand(0xB0 + i);
        oledWriteCommand(0x00);
        oledWriteCommand(0x10);

        HAL_I2C_Mem_Write(&hi2c1, OLED_I2C_ADDR, 0x40, 1, buffer[i], OLED_WIDTH, 25);
    }
}

void oledSetFont(Font *font) {
    oled.font = font;
}

uint8_t oledWriteChar(const char ch, const OLED_COLOR color) {
    uint8_t fontWidth = (oled.font)->width;
    uint8_t fontHeight = (oled.font)->height;
    if ((oled.x + fontWidth) >= OLED_WIDTH ||
            (oled.y + fontHeight) >= OLED_HEIGHT) {
        return 0;
    }

    if (ch - 32 < 0)
        return 0;

    const uint16_t *charPixels = (oled.font)->data + (ch - 32) * fontHeight;
    for (uint8_t y = 0; y < fontHeight; y++) {
        uint16_t charPixelRow = charPixels[y];
        for (uint8_t x = 0; x < fontWidth; x++) {
            if ((charPixelRow << x) & 0x8000) {
                oledSetPixel(oled.x + x, oled.y + y, color);
            } else {
                oledSetPixel(oled.x + x, oled.y + y, (OLED_COLOR) !color);
            }
        }
    }
    oled.x += fontWidth;

    return ch;
}

size_t oledWriteString(const char* const str, const OLED_COLOR color) {
    size_t count = 0;
    while (str[count] != '\0') {
        if (!oledWriteChar(str[count], color))
            break;
        count++;
    }
    return count;
}

uint8_t oledNextLine(uint8_t startX) {
    if (oled.y + oled.lineHeight >= OLED_HEIGHT)
        return 0;
    oled.y += oled.lineHeight;
    oled.x = startX;
    return 1;
}

void oledSetCursor(uint8_t x, uint8_t y) {
    oled.x = x;
    oled.y = y;
}

void oledSetLineHeight(float lineHeightRatio) {
    uint8_t fontHeight = (oled.font)->height;
    oled.lineHeight = (uint8_t) (lineHeightRatio * (float) fontHeight);
}

static void oledWriteCommand(uint8_t command) {
    HAL_I2C_Mem_Write(&hi2c1, OLED_I2C_ADDR, 0x00, 1, &command, 1, 10);
}
