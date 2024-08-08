#include <avr/io.h>
#include "notationdefines.h"
#include "../notes.h"

uint8_t voiceOne[] =
{
    INSTRUMENT(SQUARE)
    NOTE(E3, 8, 8)
    NOTE(F3, 8, 8)
};
uint8_t voiceTwo[] =
{
    INSTRUMENT(SQUARE)
    NOTE(C3, 8, 8)
    NOTE(D3, 8, 8)
};
uint8_t voiceThree[] =
{
    INSTRUMENT(SQUARE)
    NOTE(E3, 8, 8)
    NOTE(F3, 8, 8)
};
uint16_t voiceOneLength = sizeof(voiceOne) / sizeof(voiceOne[0]);
uint16_t voiceTwoLength = sizeof(voiceTwo) / sizeof(voiceTwo[0]);
uint16_t voiceThreeLength = sizeof(voiceThree) / sizeof(voiceThree[0]);
