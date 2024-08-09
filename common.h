#include <stdlib.h>
#include <stdint.h>

#ifndef COMMON_H
#define COMMON_H

#define GET_BIT(REG, BIT) (REG & (1 << BIT))
#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define UNSET_BIT(REG, BIT) (REG &= ~(1 << BIT))
#define BUILTIN_LED_ON SET_BIT(PORTB, 5)
#define BUILTIN_LED_OFF UNSET_BIT(PORTB, 5)

typedef const uint16_t sequence_t;

typedef struct state
{
    uint16_t volume;
} state;

typedef struct channel
{
    // no need for complicated device struct on pico -
    // this is simply the pwm slice number
    uint8_t device;
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
    uint8_t numChannels;
    uint8_t numTracks;

    channel *channels;
    track *tracks;
} composition;

typedef void(*instrument)(channel *channel, state *state);

#endif // COMMON_H
