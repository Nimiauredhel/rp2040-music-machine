#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdlib.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "common.h"
#include "music/musicdata.h"

static void instSilence(channel *channel, state *state);
static void instRegular(channel *channel, state *state);

static void initializePWMSlices(uint8_t first, uint8_t count, uint8_t intdiv, uint8_t fracdiv);
static void setPWMSlices(uint8_t first, uint8_t count, uint8_t value);
static void setPWMPorts(uint8_t first, uint8_t count);

static void initializeAnalogInput(void);
static float readAnalogInput(void);

static void initializeChannel(channel *channel, uint8_t device);
static channel* initializeChannels(uint8_t *numChannels);
static void initializeTrack(track *track, channel *channel,uint16_t *sequence, uint16_t sequenceLength);
static track* initializeTracks(uint8_t *numTracks, channel* channels);
static void readTrack(track *target);
static void readTracks(const uint8_t numTracks, track *tracks);
static void playChannels(const uint8_t numChannels, channel *channels, state *state);

#endif // PROGRAM_H
