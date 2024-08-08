#ifndef COMMON_H
#define COMMON_H

#include <avr/io.h>
#include <avr/pgmspace.h>

#define GET_BIT(REG, BIT) (REG & (1 << BIT))
#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define UNSET_BIT(REG, BIT) (REG &= ~(1 << BIT))
#define BUILTIN_LED_ON SET_BIT(PORTB, 5)
#define BUILTIN_LED_OFF UNSET_BIT(PORTB, 5)

typedef const PROGMEM uint16_t sequence_t;

typedef struct variableSizePointer
{
    uint8_t pointerSize;
    union pointer
    {
        volatile uint8_t *eight;
        volatile uint16_t *sixteen;
    } pointer;
} variableSizePointer;

typedef struct state
{
    uint16_t volume;
} state;

typedef struct device
{
    variableSizePointer pitch;
    variableSizePointer width;

} device;

typedef struct channel
{
    device *device;
    // the number of pitches represented by this channel
    uint8_t currentPitchCount;
    // array of pitches represented by this channel
    uint16_t currentPitches[4];
    // index of next pitch to sound on this channel
    uint8_t nextPitchIndex;
    // current "tone" (voltage?) set on this channel
    uint8_t currentTone;
    uint8_t polyCycleThreshold;
    uint8_t polyCycleCounter;
    // the "instrument" function assigned to this channel, controlling the waveform etc.
    void (*instrument)(struct channel *channel, state *state);
} channel;

typedef struct track
{
    // target channel associated with this track
    channel *channel;
    // the sequence of commands associated with this track
    const sequence_t *sequence;
    // the length of the command sequence
    uint16_t sLength;
    // the current position of the command sequence
    uint16_t sPosition;
    // the last position from which a jump was triggered
    uint16_t jPosition;
    // remaining track sleep time in microseconds
    uint64_t remainingSleepTime;
} track;

typedef struct composition
{
    uint8_t numDevices;
    uint8_t numChannels;
    uint8_t numTracks;

    device *devices;
    channel *channels;
    track *tracks;
} composition;

uint16_t VSP_Read(variableSizePointer *target);
void VSP_Write(uint16_t value, variableSizePointer *target);
typedef void(*instrument)(channel *channel, state *state);

#endif // COMMON_H
