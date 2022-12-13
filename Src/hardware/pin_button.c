#include <hardware/pin_button.h>

#include <stdlib.h>

#define PRESS_TIME_MS 50
#define RELEASE_TIME_MS 50

typedef enum ButtonState {
    RELEASE_START,
    RELEASE,
    PRESS_START,
    PRESS
} ButtonState;

struct PinButton {
    PinButtonPressHandler clickHandler;
    uint8_t enabled;
    uint32_t clickStart;
    uint32_t releaseStart;
    ButtonState state;
    GPIO_TypeDef* port;
    uint16_t pin;
};

static uint8_t pinButtonIsPressed(const PinButton *button);

PinButton * pinButtonCreate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    PinButton * button = (PinButton *) malloc(sizeof(PinButton));
    button->enabled = 1;
    button->clickHandler = NULL;
    button->state = RELEASE;
    button->pin = GPIO_Pin;
    button->port = GPIOx;
    return button;
}

void pinButtonSetPressHandler(PinButton *pinButton, PinButtonPressHandler pressHandler) {
    pinButton->clickHandler = pressHandler;
}

void pinButtonDisable(PinButton * const button) {
    button->enabled = 0;
}

void pinButtonEnable(PinButton * const button) {
    button->enabled = 1;
}

uint8_t pinButtonIsEnabled(const PinButton * const button) {
    return button->enabled;
}

void pinButtonPolling(PinButton * const button) {
    if (!button->enabled)
        return;
    uint32_t time = HAL_GetTick();
    uint8_t isPressed = pinButtonIsPressed(button);

    switch (button->state) {
        case RELEASE_START:
            if (isPressed) {
                button->state = PRESS;
            } else if (time - button->releaseStart > RELEASE_TIME_MS) {
                button->state = RELEASE;
            }
            break;
        case RELEASE:
            if (isPressed) {
                button->state = PRESS_START;
                button->clickStart = time;
            }
            break;
        case PRESS_START:
            if (!isPressed) {
                button->state = RELEASE;
            } else if (time - button->clickStart > PRESS_TIME_MS) {
                button->state = PRESS;
                if (button->clickHandler != NULL)
                    button->clickHandler();
            }
            break;
        case PRESS:
            if (!isPressed) {
                button->state = RELEASE_START;
                button->releaseStart = time;
            }
            break;
    }
}

static uint8_t pinButtonIsPressed(const PinButton *const button) {
	return HAL_GPIO_ReadPin(button->port, button->pin) == GPIO_PIN_RESET ? 1 : 0;
}




