#include "musicdata.h"
uint16_t voiceOne[] =
{
    TEMPO(56)
    INSTRUMENT(SQUARE)
    NOTE(C3, 48, 32)
    NOTE(G3, 48, 32)
};
uint16_t voiceTwo[] =
{
    TEMPO(56)
    SILENCE(32)
    SILENCE(32)
};
uint16_t voiceThree[] =
{
    TEMPO(56)
    SILENCE(32)
    SILENCE(32)
};
const uint16_t voiceOneLength = sizeof(voiceOne) / sizeof(voiceOne[0]);
const uint16_t voiceTwoLength = sizeof(voiceTwo) / sizeof(voiceTwo[0]);
const uint16_t voiceThreeLength = sizeof(voiceThree) / sizeof(voiceThree[0]);
