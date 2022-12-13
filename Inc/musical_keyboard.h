#ifndef MUSICAL_KEYBOARD_MUSICALKEYBOARD_H
#define MUSICAL_KEYBOARD_MUSICALKEYBOARD_H

#include <stdint.h>

#include "hardware/buffer.h"

typedef enum Note {
    DO = 0,
    RE,
    MI,
    FA,
    SOL,
    LA,
    SI
} Note;

void musicalKeyboardInit();
void musicalKeyboardNextOctave();
void musicalKeyboardPrevOctave();
void musicalKeyboardPlayNote(Note note);
void musicalKeyboardReduceDuration(uint16_t deltaMs);
void musicalKeyboardIncreaseDuration(uint16_t deltaMs);
void musicalKeyboardPlayCurrentOctave();
uint16_t musicalKeyboardGetCurrentDuration();
uint8_t musicalKeyboardGetCurrentOctave();
uint16_t musicalKeyboardGetCurrentNote();
const uint16_t * musicalKeyboardGetCurrentNotes();
void musicalKeyboardDebugEnable();
void musicalKeyboardDebugDisable();
Buffer * musicalKeyboardGetDebugBuffer();

static struct {
    void (*init)();
    void (*nextOctave)();
    void (*prevOctave)();
    void (*playNote)(Note note);
    void (*reduceDuration)(uint16_t deltaMs);
    void (*increaseDuration)(uint16_t deltaMs);
    void (*playCurrentOctave)();
    uint16_t (*currentDuration)();
    uint8_t (*currentOctave)();
    uint16_t (*currentNote)();
    const uint16_t *(*currentNotes)();
    void (*debugEnable)();
    void (*debugDisable)();
    Buffer * (*getDebugBuffer)();
} const MusicalKeyboard = {
        musicalKeyboardInit,
        musicalKeyboardNextOctave,
        musicalKeyboardPrevOctave,
        musicalKeyboardPlayNote,
        musicalKeyboardReduceDuration,
        musicalKeyboardIncreaseDuration,
        musicalKeyboardPlayCurrentOctave,
        musicalKeyboardGetCurrentDuration,
        musicalKeyboardGetCurrentOctave,
        musicalKeyboardGetCurrentNote,
        musicalKeyboardGetCurrentNotes,
        musicalKeyboardDebugEnable,
        musicalKeyboardDebugDisable,
        musicalKeyboardGetDebugBuffer
};


#endif //MUSICAL_KEYBOARD_MUSICALKEYBOARD_H
