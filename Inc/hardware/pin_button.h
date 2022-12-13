#ifndef MUSICAL_KEYBOARD_PINBUTTON_H
#define MUSICAL_KEYBOARD_PINBUTTON_H

#include <stdint.h>

#include "stm32f4xx_hal.h"


typedef struct PinButton PinButton;

typedef void (*PinButtonPressHandler)();

void pinButtonSetPressHandler(PinButton *pinButton, PinButtonPressHandler pressHandler);
PinButton * pinButtonCreate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void pinButtonEnable(PinButton *pinButton);
void pinButtonDisable(PinButton *pinButton);
uint8_t pinButtonIsEnabled(const PinButton *pinButton);
void pinButtonPolling(PinButton * const button);

static const struct {
    void (*setPressHandler)(PinButton *pinButton, PinButtonPressHandler pressHandler);
    PinButton * (*create)(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
    void (*enable)(PinButton *pinButton);
    void (*disable)(PinButton *pinButton);
    uint8_t (*isEnabled)(const PinButton *pinButton);
    void (*polling)(PinButton *pinButton);
} pinButton = {
        pinButtonSetPressHandler,
        pinButtonCreate,
        pinButtonEnable,
        pinButtonDisable,
        pinButtonIsEnabled,
        pinButtonPolling,
};

#endif //MUSICAL_KEYBOARD_PINBUTTON_H
