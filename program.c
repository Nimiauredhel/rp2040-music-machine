#include <stdlib.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "program.h"
#include "common.h"
#include "music/musicdata.c"

static uint64_t sleepUnit = 512;
static uint8_t rhythmUnit = 255; // also set dynamically by tracks

#define MINIMAL_DELAY sleep_us(10);
#define SLEEP_DELAY sleep_us(sleepUnit);

static void instSilence(channel *channel, state *state)
{
}

static void instRegular(channel *channel, state *state)
{
    if (channel->currentPitchCount == 0) return;

    if (channel->nextPitchIndex >= channel->currentPitchCount)
    {
        channel->nextPitchIndex = 0;
    }

    uint8_t finalTone = ((channel->currentTone)*5*(state->volume))/1024;
    pwm_set_chan_level(channel->device, PWM_CHAN_A, finalTone);
    pwm_set_wrap(channel->device, channel->currentPitches[channel->nextPitchIndex]*5);

    channel->polyCycleCounter++;

    if (channel->polyCycleCounter >= channel->polyCycleThreshold)
    {
        channel->polyCycleCounter = 0;
        if (channel->nextPitchIndex < channel->currentPitchCount-1)
        {
            channel->nextPitchIndex++;
        }
        else if (channel->nextPitchIndex == channel->currentPitchCount-1)
        {
            channel->nextPitchIndex = 0;
        }
    }
}

const instrument instruments[] =
{
    instSilence, instRegular
};

static void initializePWMSlices(uint8_t first, uint8_t count, uint8_t intdiv, uint8_t fracdiv)
{
    for (int i = first; i < count; i++)
    {
        pwm_set_clkdiv_int_frac(i, intdiv, fracdiv);
        pwm_set_phase_correct(i, true);
    }
}
static void setPWMSlices(uint8_t first, uint8_t count, uint8_t value)
{
    for (int i = first; i < count; i++)
    {
        pwm_set_enabled(i, value);
    }
}
static void setPWMPorts(uint8_t first, uint8_t count)
{
    for (int i = first; i < count; i++)
    {
        gpio_set_function(i, GPIO_FUNC_PWM);
    }
}

static void initializeAnalogInput(void)
{
/*
    ADMUX = 0;
    ADCSRA = 0;
    // reference voltage is vcc (5v?), 10 bit mode, using pin A0
    ADMUX |= (1 << REFS0) | (0 << ADLAR);
    // enable ADC module, set prescaler divisor value to 128
    ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
*/
}

static uint16_t readAnalogInput()
{
/*
    SET_BIT(ADCSRA, ADSC);

    while (GET_BIT(ADCSRA, ADSC) == 1)
    {
        MINIMAL_DELAY
    }

    return ADC;
*/
    return 1023;
}

static void initializeChannel(channel *channel, uint8_t device)
{
    channel->device = device;
    channel->currentTone = 0;
    for(int i = 0; i < 4; i++)
    {
        channel->currentPitches[i] = 255;
    }
    channel->currentPitchCount = 0;
    channel->nextPitchIndex = 0;
    channel->polyCycleThreshold = 96;
    channel->polyCycleCounter = 0;
    channel->instrument = instruments[0];
}

static channel* initializeChannels(uint8_t *numChannels)
{
    *numChannels = 3;
    channel *channels = malloc((*numChannels) * sizeof(channel));

    for (int i = 0; i < *numChannels; i++)
    {
        // add 2 for first port test, use whole range later
        // TODO: remove +2
        initializeChannel(&channels[i], i + 2);
    }
    return channels;
}

static void initializeTrack(track *track, channel *channel, uint16_t *sequence, const uint16_t sequenceLength)
{
    track->channel = channel;
    track->sequence = sequence;
    track->sLength = sequenceLength;
    track->sPosition = 0;
    track->remainingSleepTime = 0;
    track->jPosition = 0;
}
static track* initializeTracks(uint8_t *numTracks, channel* channels)
{
    *numTracks = 3;
    track *tracks = malloc((*numTracks) * sizeof(track));
    initializeTrack(&tracks[0], &channels[0], voiceOne, voiceOneLength);
    initializeTrack(&tracks[1], &channels[1], voiceTwo, voiceTwoLength);
    initializeTrack(&tracks[2], &channels[2], voiceThree, voiceThreeLength);
    return tracks;
}

static void readTrack(track *target)
{
    if (target->remainingSleepTime > 0)
    {
        target->remainingSleepTime-=sleepUnit;
        return;
    }
    
    if (target->sPosition >= target->sLength - 1)
    {
        target->remainingSleepTime = 0;
        return;
    }

    const uint16_t *tSequence = target->sequence;
    uint16_t position = target->sPosition;
    uint8_t code = tSequence[position];
    channel *tChannel = target->channel;

    switch (code) 
    {
        case 0:
            // sleep for duration
            target->remainingSleepTime =
                tSequence[position+1] * sleepUnit * rhythmUnit;
            target->sPosition = position+2;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            // Set pitches
            // pitches + sleep combo to save space
            target->remainingSleepTime =
                tSequence[position+code+1] * sleepUnit * rhythmUnit;
            tChannel->currentPitchCount = code;
            target->sPosition = position + code + 2;
            for (int i = 0; i < code; i++)
            {
                tChannel->currentPitches[i] = tSequence[position+i+1];
            }
            tChannel->nextPitchIndex = 0;
            break;
        case 11:
        case 12:
        case 13:
        case 14:
            code -= 10;
            // Set pitches
            // pitches + sleep + volume combo to save space
            target->channel->currentTone = tSequence[position+code+1];
            target->remainingSleepTime =
                tSequence[position+code+2] * sleepUnit * rhythmUnit;
            target->sPosition = position + code + 3;
            tChannel->currentPitchCount = code;
            for (int i = 0; i < code; i++)
            {
                tChannel->currentPitches[i] = tSequence[position+i+1];
            }
            tChannel->nextPitchIndex = 0;
            break;
        case 5:
            // Set "volume" (voltage)
            tChannel->currentTone =
                tSequence[position+1];
            target->sPosition = position+2;
            break;
        case 6:
            // Set instrument function
            tChannel->instrument =
                instruments[tSequence[position+1]];
            target->sPosition = position+2;
            break;
        case 7:
            // Set rhythm unit (tempo)
            rhythmUnit =
                tSequence[position+1];
            target->sPosition = position+2;
            break;
        case 8:
            // Jump back (repeat)
            if (target->jPosition == position)
            {
                target->sPosition = position+2;
                target->jPosition = 0;
            }
            else
            {
                target->jPosition = position;
                target->sPosition = position-(tSequence[position+1]);
            }
            break;
        case 9:
            // volume + sleep
            tChannel->currentTone =
                tSequence[position+1];
            target->remainingSleepTime =
                tSequence[position+2] * sleepUnit * rhythmUnit;
            target->sPosition = position+3;
            break;
        default:
            // TODO: add some sort of warning behavior
        break;
    }
}

static void readTracks(const uint8_t numTracks, track *tracks)
{
    // check if it's time to loop
    // and manually resync tracks
    for (int i = 0; i < numTracks; i++)
    {
        if (tracks[i].sPosition >= tracks[i].sLength-1
            && tracks[i].remainingSleepTime <= 0)
        {
            // if true for ONE, sync ALL then continue!
            for(int j = 0; j < numTracks; j++)
            {
                tracks[j].sPosition = 0;
                tracks[j].jPosition = 0;
                tracks[j].remainingSleepTime = 0;
            }
            break;
        }
    }
    
    // proceed to execute track commands
    for (int i = 0; i < numTracks; i++)
    {
        readTrack(&tracks[i]);
    }
}

static void playChannels(const uint8_t numChannels, channel *channels, state *state)
{
    for (int i = 0; i < numChannels; i++)
    {
        channels[i].instrument(&channels[i], state);
    }
}

int main(void)
{
    state *state = malloc(sizeof(struct state));
    composition *composition = malloc(sizeof(struct composition));

    initializePWMSlices(0, 5, 255, 15);
    setPWMSlices(0, 5, true);
    setPWMPorts(0, 10);

    initializeAnalogInput();

    // initialize the channels - device usage & state management
    composition->channels = initializeChannels(&composition->numChannels);
    // initialize the tracks - parallel streams of commands to the channels
    composition->tracks = initializeTracks(&composition->numTracks, composition->channels);

    for(;;)
    {
        state->volume = readAnalogInput();
        readTracks(composition->numTracks, composition->tracks);
        playChannels(composition->numChannels, composition->channels, state);
        SLEEP_DELAY
    }
}
