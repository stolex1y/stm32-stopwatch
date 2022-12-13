#ifndef MUSICAL_KEYBOARD_BUZZER_H
#define MUSICAL_KEYBOARD_BUZZER_H

#include <stdint.h>

#include "stm32f4xx_hal.h"

void buzzerInit();
void buzzerPlayNote(uint16_t freq, uint16_t durationMs);
void buzzerPlayNotes(const uint16_t freq[], uint16_t size, const uint16_t durationMs[]);

static struct BuzzerInterface {
    void (*init)();
    void (*playNote)(uint16_t freq, uint16_t durationMs);
    void (*playNotes)(const uint16_t freq[], uint16_t size, const uint16_t durationMs[]);
} const Buzzer = {
        buzzerInit,
        buzzerPlayNote,
        buzzerPlayNotes
};

#endif //MUSICAL_KEYBOARD_BUZZER_H
