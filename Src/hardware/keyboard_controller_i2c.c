#include <hardware/keyboard_controller_i2c.h>

#include "i2c.h"

#define RECEIVER_ADDR 0xE2

typedef enum {
	INPUT = 0x00,
	OUTPUT = 0x01,
	POLARITY_INVERSION = 0x02,
	CONFIG = 0x03
} KeyboardRegisters;

HAL_StatusTypeDef keyboardControllerReadInput(uint8_t * buf) {
	return HAL_I2C_Mem_Read(&hi2c1, RECEIVER_ADDR | 1, INPUT, 1, buf, 1, 100);
}

HAL_StatusTypeDef keyboardControllerWriteConfig(uint8_t config) {
	return HAL_I2C_Mem_Write(&hi2c1, RECEIVER_ADDR & 0xFFFE, CONFIG, 1, &config, 1, 100);
}

HAL_StatusTypeDef keyboardControllerWriteOutput(uint8_t value) {
	return HAL_I2C_Mem_Write(&hi2c1, RECEIVER_ADDR & 0xFFFE, OUTPUT, 1, &value, 1, 100);
}

