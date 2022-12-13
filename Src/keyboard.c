#include <keyboard.h>

#include "hardware/keyboard_controller_i2c.h"
#include "hardware/keyboard_button.h"
#include "hardware/buffer.h"

#define KB_HEIGHT 4
#define KB_WIDTH 3
#define DEBUG_BUF_SIZE 100

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static const KeyboardKeyName keyboardKeysNames[KB_HEIGHT][KB_WIDTH] = {
        { KEY_1, KEY_2, KEY_3 },
        { KEY_4, KEY_5, KEY_6 },
        { KEY_7, KEY_8, KEY_9 },
        { KEY_A, KEY_B, KEY_C },
};

static struct Keyboard {
    KeyboardButton * keys[KB_HEIGHT * KB_WIDTH];
    Buffer * buffer;
    uint8_t debugModeEnabled;
} keyboard = {};

static void updateDebugMessages();
static HAL_StatusTypeDef checkNthKeyboardRow(uint8_t n);
static uint8_t isButtonNthColumnPressed(uint8_t kbState, uint8_t n);

void keyboardInit() {
    for (uint8_t i = 0; i < KB_HEIGHT; i++) {
        for (uint8_t j = 0; j < KB_WIDTH; j++) {
            KeyboardKeyName keyName = keyboardKeysNames[i][j];
            KeyboardButton *key = keyboardButton.create(keyName);
            keyboard.keys[keyName] = key;
        }
    }
    keyboard.buffer = buffer.create(DEBUG_BUF_SIZE);
}

uint8_t keyboardStatePolling() {
    uint8_t pressedKeys[KB_HEIGHT][KB_WIDTH];
    uint8_t pressedKeysCount = 0;
    for (uint8_t row = 0; row < KB_HEIGHT; row++) {
    	HAL_StatusTypeDef i2cStatus = checkNthKeyboardRow(row);
    	if (i2cStatus)
    		return 1;
    	i2cStatus = KeyboardControllerI2C.writeOutput(0x0);
        uint8_t kbState = 0xFF;
        i2cStatus = KeyboardControllerI2C.readInput(&kbState);
        for (uint8_t column = 0; column < KB_WIDTH; column++) {
            uint8_t keyIsPressed = isButtonNthColumnPressed(kbState, column);
            pressedKeys[row][column] = keyIsPressed;
            pressedKeysCount = keyIsPressed ? pressedKeysCount + 1 : pressedKeysCount;
        }
    }
    for (uint8_t row = 0; row < KB_HEIGHT; row++) {
        for (uint8_t column = 0; column < KB_WIDTH; column++) {
            KeyboardKeyName keyName = keyboardKeysNames[row][column];
            KeyboardButton *key = keyboard.keys[keyName];
            if (pressedKeysCount > 1)
                keyboardButton.updateState(key, 0);
            else
                keyboardButton.updateState(key, pressedKeys[row][column]);
        }
    }
    if (keyboard.debugModeEnabled)
        updateDebugMessages();
    return 0;
}

void keyboardSetKeyPressHandler(KeyboardKeyName key, KeyboardButtonPressHandler keyPressHandler) {
    keyboardButton.setPressHandler(keyboard.keys[key], keyPressHandler);
}

void keyboardDebugEnable() {
    keyboard.debugModeEnabled = 1;
    for (uint8_t i = 0; i < KB_WIDTH * KB_HEIGHT; i++) {
        keyboardButton.debugEnable(keyboard.keys[i]);
    }
}

void keyboardDebugDisable() {
    keyboard.debugModeEnabled = 0;
    for (uint8_t i = 0; i < KB_WIDTH * KB_HEIGHT; i++) {
        keyboardButton.debugDisable(keyboard.keys[i]);
    }
    buffer.clear(keyboard.buffer);
}

Buffer * keyboardGetDebugBuffer() {
    return keyboard.buffer;
}

static HAL_StatusTypeDef checkNthKeyboardRow(uint8_t n) {
    return KeyboardControllerI2C.writeConfig(~(1 << n));
}

static uint8_t isButtonNthColumnPressed(uint8_t kbState, uint8_t n) {
    return !(kbState & (0x10 << n));
}

static void updateDebugMessages() {
    for (uint8_t i = 0; i < KB_HEIGHT * KB_WIDTH; i++) {
        if (buffer.freeSize(keyboard.buffer) == 0)
            break;
        KeyboardButton * key = keyboard.keys[i];
        buffer.popToBuffer(keyboardButton.getDebugBuffer(key), keyboard.buffer, buffer.freeSize(keyboard.buffer));
    }
}
