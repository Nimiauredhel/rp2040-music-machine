#include <avr/io.h>
#include "notationdefines.h"
#include "../notes.h"

uint32_t voiceOne[] =
{
    TEMPO(256)
                    INSTRUMENT(SQUARE)
                    //
            VOLUME(32)
    PITCH3S(F4, Ab4, C5, 18)
    PITCH3S(G4, Bb5, D5, 6)
            JUMPBACK(10)
    PITCH3S(Ab4, C5, Eb5, 18)
    PITCH3S(G4, Bb5, D5, 6)
    PITCH3S(F4, Ab4, C5, 24)
//
    PITCH3S(C4, Ab4, Ab4, 18)
    PITCH3S(Ab3, C4, F4, 6)
    PITCH3S(D4, G4, Bb4, 18)
    PITCH3S(C4, Eb4, Ab4, 6)
                            //
    PITCH3S(C4, Eb4, C5, 6)
    PITCH3S(C4, Eb4, Db5, 6)
    PITCH3S(Bb3, D4, Bb4, 9)
    PITCH3S(Bb3, D4, Ab4, 3)
    PITCH3S(F3, Ab3, C5, 24)
                            //
    PITCH3S(Ab4, B4, Eb5, 18)
    PITCH3S(Bb4, Db5, F5, 6)
            JUMPBACK(10)
    PITCH3S(B4, Eb5, Gb5, 18)
    PITCH3S(B4, Eb5, Ab5, 6)
    PITCH3S(F4, Bb4, Eb5, 12)
    PITCH3S(F4, Bb4, D5, 12)
                            //
    PITCH3S(Ab4, B4, Eb5, 18)
    PITCHS(Ab4, 2) 
    PITCHS(B4, 2) 
    PITCHS(Eb5, 2) 
    PITCHS(Gb5, 6)
    PITCHS(F5, 6)
    PITCHS(Db5, 6) 
    PITCHS(Bb4, 6) 
    PITCH3S(Gb4, Bb5, Eb5, 48)
};
uint32_t voiceTwo[] =
{
    TEMPO(256)
    INSTRUMENT(SQUARE)
    VOLUME(1)
//
    PITCHS(C3, 18)
    PITCHS(D3, 6)
    JUMPBACK(6)
    PITCHS(Eb3, 18)
    PITCHS(D3, 6)
    PITCHS(C3, 24)
//
    PITCHS(F3, 24)
    PITCHS(G3, 18)
    PITCHS(Ab3, 6)
                            //
    PITCHS(Ab3, 12)
    PITCHS(G3, 12)
    PITCHS(C3, 24)
                            //
    PITCHS(Eb3, 18)
    PITCHS(F3, 6)
            JUMPBACK(6)
    PITCHS(Gb3, 24)
    PITCHS(D3, 24)
                            //
    PITCHS(Eb3, 18)
    PITCHS(Ab3, 2) 
    PITCHS(Ab4, 2) 
    PITCHS(B4, 2) 
    PITCH3S(Db4, Ab4, Db5, 24)
    PITCHS(Eb4, 24)
    PITCHS(Eb3, 24)
};
const uint16_t voiceOneLength = sizeof(voiceOne) / sizeof(voiceOne[0]);
const uint16_t voiceTwoLength = sizeof(voiceTwo) / sizeof(voiceTwo[0]);
