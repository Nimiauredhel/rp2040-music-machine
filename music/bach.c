#include "../notes.h"
#include "notationdefines.h""
#include <avr/io.h>

uint32_t sequence[] =
{
TEMPO(6400)
VOLUME(1)
    INSTRUMENT(SQUARE)
SLEEP(2)
VOLUME(2)
SLEEP(2)
VOLUME(3)
SLEEP(2)
VOLUME(4)
SLEEP(2)
VOLUME(5)
    PITCHS(Gb5, 1)
VOLUME(10)
SLEEP(1)
    INSTRUMENT(SQUARE)
VOLUME(15)
SLEEP(1)
VOLUME(20)
SLEEP(1)

TEMPO(4800)

VOLUME(21)
    PITCHS(B4, 4)
VOLUME(22)
    PITCHS(D5, 4)
VOLUME(23)
    PITCHS(Gb5, 4)
VOLUME(24)
    PITCHS(B5, 4)

VOLUME(25)
    PITCHS(G5, 4)
    PITCHS(E5, 4)
    PITCHS(Db5, 4)
    PITCHS(E5, 4)

    PITCHS(Bb4, 4)
    PITCHS(G5, 4)
    PITCHS(Gb5, 4)
    PITCHS(E5, 4)

    PITCHS(D5, 4)
    PITCHS(B4, 4)
    PITCHS(Gb4, 4)
    PITCHS(A4, 4)

    PITCHS(G4, 4)
    PITCHS(Gb5, 4)
    PITCHS(E5, 4)
    PITCHS(D5, 4)
    
    PITCHS(Db5, 4)
    PITCHS(E5, 4)
    PITCHS(G5, 4)
    PITCHS(B4, 4)

TEMPO(5600)

    PITCHS(Gb4, 4)
    PITCHS(Db5, 4)
    PITCHS(Gb4, 4)
    PITCHS(E4, 4)

TEMPO(4800)
VOLUME(10)
    PITCHS(D4, 4)
VOLUME(15)
    PITCHS(B4, 4)
VOLUME(20)
    PITCHS(Db4, 4)
VOLUME(25)
    PITCHS(Db5, 4)
 //   PITCHS2(D4, B4, 4)
 //   PITCHS2(Db4, Db5, 4)

    // line 2

VOLUME(30)
    PITCHS(D5, 4)
    PITCHS(Gb4, 4)
VOLUME(1)
    PITCHS(B3, 4)
VOLUME(2)
    PITCHS(Db4, 4)
             
VOLUME(3)
    PITCHS(D4, 4)
VOLUME(4)
    PITCHS(E4, 4)
VOLUME(5)
    PITCHS(Gb4, 4)
    PITCHS(G4, 4)
VOLUME(10)
    PITCHS(A4, 4)
    PITCHS(D4, 4)
    PITCHS(Gb4, 4)
    PITCHS(A4, 4)
             
    PITCHS(D5, 4)
    PITCHS(E5, 4)
    PITCHS(Gb5, 4)
    PITCHS(A4, 4)

    PITCHS(B4, 4)
    PITCHS(G4, 4)
    PITCHS(B4, 4)
    PITCHS(D5, 4)
             
    PITCHS(G5, 4)
    PITCHS(B4, 4)
    PITCHS(G4, 4)
    PITCHS(E4, 4)
             
TEMPO(5600)
VOLUME(20)
    PITCHS(Db4, 4)
    PITCHS(E4, 4)
    PITCHS(A4, 4)
    PITCHS(G5, 4)
             
TEMPO(6400)
    PITCHS(Gb5, 4)
    PITCHS(D4, 4)
    PITCHS(G4, 4)
    PITCHS(E5, 4)
};
