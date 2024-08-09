#include "musicdata.h"
uint16_t voiceOne[] =
{
    INSTRUMENT(SQUARE)
    NOTE(C3, 8, 256)
    NOTE(G3, 8, 256)
};
uint16_t voiceTwo[] =
{
    INSTRUMENT(SQUARE)
    NOTE(C3, 8, 256)
    NOTE(G3, 8, 256)
};
uint16_t voiceThree[] =
{
    INSTRUMENT(SQUARE)
    NOTE(C3, 8, 256)
    NOTE(G3, 8, 256)
};
const uint16_t voiceOneLength = sizeof(voiceOne) / sizeof(voiceOne[0]);
const uint16_t voiceTwoLength = sizeof(voiceTwo) / sizeof(voiceTwo[0]);
const uint16_t voiceThreeLength = sizeof(voiceThree) / sizeof(voiceThree[0]);
