#ifndef STOPWATCH_STOPWATCH_H
#define STOPWATCH_STOPWATCH_H

#include <stdint.h>

typedef struct Time {
    uint32_t min;
    uint32_t sec;
    uint32_t ms;
} Time;

typedef struct Stopwatch Stopwatch;

Stopwatch * stopwatchCreate();
void stopwatchStart(Stopwatch *stopwatch);
void stopwatchReset(Stopwatch *stopwatch);
Time stopwatchGetCurrentTime(const Stopwatch *stopwatch);
void stopwatchPause(Stopwatch *stopwatch);
uint8_t stopwatchIsEnabled(const Stopwatch *stopwatch);

const static struct {
    Stopwatch * (*create)();
    void (*start)(Stopwatch *stopwatch);
    void (*reset)(Stopwatch *stopwatch);
    Time (*getCurrentTime)(const Stopwatch *stopwatch);
    void (*pause)(Stopwatch *stopwatch);
    uint8_t (*isEnabled)(const Stopwatch *stopwatch);
} stopwatch = {
        stopwatchCreate,
        stopwatchStart,
        stopwatchReset,
        stopwatchGetCurrentTime,
        stopwatchPause,
        stopwatchIsEnabled,
};

#endif //STOPWATCH_STOPWATCH_H
