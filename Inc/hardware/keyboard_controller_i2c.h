#ifndef MUSICAL_KEYBOARD_KEYBOARDCONTROLLERI2C_H
#define MUSICAL_KEYBOARD_KEYBOARDCONTROLLERI2C_H

#include <stdint.h>

#include "i2c.h"

HAL_StatusTypeDef keyboardControllerReadInput(uint8_t * buf);
HAL_StatusTypeDef keyboardControllerWriteConfig(uint8_t config);
HAL_StatusTypeDef keyboardControllerWriteOutput(uint8_t value);

static struct {
	HAL_StatusTypeDef (*readInput)(uint8_t * buf);
    HAL_StatusTypeDef (*writeConfig)(uint8_t config);
    HAL_StatusTypeDef (*writeOutput)(uint8_t value);
} const KeyboardControllerI2C = {
        keyboardControllerReadInput,
        keyboardControllerWriteConfig,
		keyboardControllerWriteOutput
};

#endif //MUSICAL_KEYBOARD_KEYBOARDCONTROLLERI2C_H
