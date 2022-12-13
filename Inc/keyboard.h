#ifndef MUSICAL_KEYBOARD_KEYBOARD_H
#define MUSICAL_KEYBOARD_KEYBOARD_H

#include <stdint.h>

#include "hardware/keyboard_button.h"
#include "hardware/buffer.h"

typedef enum {
    KEY_1 = 0,
    KEY_2 = 1,
    KEY_3 = 2,
    KEY_4 = 3,
    KEY_5 = 4,
    KEY_6 = 5,
    KEY_7 = 6,
    KEY_8 = 7,
    KEY_9 = 8,
    KEY_A = 9,
    KEY_B = 10,
    KEY_C = 11
} KeyboardKeyName;

void keyboardSetKeyPressHandler(KeyboardKeyName key, KeyboardButtonPressHandler keyPressHandler);
uint8_t keyboardStatePolling();
void keyboardInit();
void keyboardDebugEnable();
void keyboardDebugDisable();
Buffer * keyboardGetDebugBuffer();

static struct {
    void (*setKeyPressHandler)(KeyboardKeyName key, KeyboardButtonPressHandler keyPressHandler);
    uint8_t (*statePolling)();
    void (*init)();
    void (*debugEnable)();
    void (*debugDisable)();
    Buffer * (*getDebugBuffer)();
} const Keyboard = {
        keyboardSetKeyPressHandler,
        keyboardStatePolling,
        keyboardInit,
        keyboardDebugEnable,
        keyboardDebugDisable,
        keyboardGetDebugBuffer
};

#endif //MUSICAL_KEYBOARD_KEYBOARD_H
