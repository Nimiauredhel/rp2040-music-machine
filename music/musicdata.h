#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "../common.h"
#include "notationdefines.h"
#include "../notes.h"

extern sequence_t voiceOne[];
extern sequence_t voiceTwo[];
extern sequence_t voiceThree[];
extern const uint16_t PROGMEM voiceOneLength;
extern const uint16_t PROGMEM voiceTwoLength;
extern const uint16_t PROGMEM voiceThreeLength;

#endif
