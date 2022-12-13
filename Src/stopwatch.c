#include "stopwatch.h"

#include "stm32f4xx_hal.h"

#include <stdlib.h>

#define SEC_TO_MS ((uint32_t) 1000)
#define MIN_TO_MS ((uint32_t) 60 * (uint32_t) SEC_TO_MS)

typedef struct Stopwatch {
    uint32_t startTime;
    uint32_t stopTime;
    uint8_t enabled;
    uint8_t reset;
} Stopwatch;

static Time msToTime(uint32_t ms);

Stopwatch * stopwatchCreate() {
    Stopwatch *stopwatch = (Stopwatch *) malloc(sizeof(Stopwatch));
    stopwatch->enabled = 0;
    stopwatch->startTime = 0;
    stopwatch->stopTime = 0;
    return stopwatch;
}

void stopwatchStart(Stopwatch *stopwatch) {
    if (stopwatch->enabled || stopwatch->reset) {
        stopwatch->startTime = HAL_GetTick();
    } else {
        stopwatch->startTime = HAL_GetTick() - (stopwatch->stopTime - stopwatch->startTime);
    }
    stopwatch->reset = 0;
    stopwatch->enabled = 1;
}

void stopwatchReset(Stopwatch *stopwatch) {
    stopwatch->enabled = 0;
    stopwatch->reset = 1;
    stopwatch->startTime = HAL_GetTick();
    stopwatch->stopTime = stopwatch->startTime;
}

uint8_t stopwatchIsEnabled(const Stopwatch *stopwatch) {
    return stopwatch->enabled;
}

void stopwatchPause(Stopwatch *stopwatch) {
    if (stopwatch->enabled) {
        stopwatch->enabled = 0;
        stopwatch->stopTime = HAL_GetTick();
    }
}

Time stopwatchGetCurrentTime(const Stopwatch *stopwatch) {
    if (stopwatch->enabled)
        return msToTime(HAL_GetTick() - stopwatch->startTime);
    else
        return msToTime(stopwatch->stopTime - stopwatch->startTime);
}

static Time msToTime(uint32_t ms) {
    return (Time) {
        .min = ms / MIN_TO_MS,
        .sec = ms % MIN_TO_MS / SEC_TO_MS,
        .ms = ms % SEC_TO_MS
    };
}
