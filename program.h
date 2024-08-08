#ifndef PROGRAM_H
#define PROGRAM_H

#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "common.h"
#include "music/musicdata.h"

static void instSilence(channel *channel, state *state);
static void instRegular(channel *channel, state *state);

static void initializeTimerCounter0(void);
static void initializeTimerCounter1(void);
static void initializeTimerCounter2(void);
static void startTimerCounter0(void);
static void startTimerCounter1(void);
static void startTimerCounter2(void);
static void stopTimerCounter0(void);
static void stopTimerCounter1(void);
static void stopTimerCounter2(void);

static void initializePortB(void);
static void initializePortD(void);
static void initializeAnalogInput(void);
static uint16_t readAnalogInput(void);

static void initializeDevice8(device *device, volatile uint8_t *pitch, volatile uint8_t *width);
static void initializeDevice16(device *device, volatile uint16_t *pitch, volatile uint16_t *width);
static device* initializeDevices(uint8_t *numDevices);
static void initializeChannel(channel *channel, device *device);
static channel* initializeChannels(uint8_t *numChannels, device *devices);
static void initializeTrack(track *track, channel *channel, sequence_t *sequence, uint16_t sequenceLength);
static track* initializeTracks(uint8_t *numTracks, channel* channels);
static void readTrack(track *target);
static void readTracks(const uint8_t numTracks, track *tracks);
static void playChannels(const uint8_t numChannels, channel *channels, state *state);

#endif // PROGRAM_H
