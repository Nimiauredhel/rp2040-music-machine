#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "program.h"
#include "common.c"
#include "music/musicdata.c"

static uint64_t sleepUnit = 512;
static uint8_t rhythmUnit = 255; // also set dynamically by tracks

#define SLEEP_DELAY _delay_us(sleepUnit);

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

    uint8_t finalTone = ((channel->currentTone)*(state->volume))/1024;
    VSP_Write(finalTone, &channel->device->width);
    VSP_Write(channel->currentPitches[channel->nextPitchIndex], 
            &channel->device->pitch);

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

// timer counter 0
static void initializeTimerCounter0(void)
{
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
    TCCR0B = (1 << WGM02); 
}
static void startTimerCounter0(void)
{
    // 1 0 0 - clk/256
    //SET_BIT(TCCR0B, CS00);
    //SET_BIT(TCCR0B, CS01);
    SET_BIT(TCCR0B, CS02);
}
static void stopTimerCounter0(void)
{
    //UNSET_BIT(TCCR0B, CS00);
    //UNSET_BIT(TCCR0B, CS01);
    UNSET_BIT(TCCR0B, CS02);
}
// timer counter 1
static  void initializeTimerCounter1(void)
{
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << WGM13); 
}
static void startTimerCounter1(void)
{
    // 1 0 0 - clk/256
    //SET_BIT(TCCR1B, CS10);
    //SET_BIT(TCCR1B, CS11);
    SET_BIT(TCCR1B, CS12);
}
static void stopTimerCounter1(void)
{
    //UNSET_BIT(TCCR1B, CS10);
    //UNSET_BIT(TCCR1B, CS11);
    UNSET_BIT(TCCR1B, CS12);
}
// timer counter 2
static void initializeTimerCounter2(void)
{
    TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20);
    TCCR2B = (1 << WGM22); 
}
static void startTimerCounter2(void)
{
    // 1 0 0 - clk/256
    //SET_BIT(TCCR2B, CS20);
    SET_BIT(TCCR2B, CS21);
    SET_BIT(TCCR2B, CS22);
}
static void stopTimerCounter2(void)
{
    //UNSET_BIT(TCCR2B, CS20);
    UNSET_BIT(TCCR2B, CS21);
    UNSET_BIT(TCCR2B, CS22);
}

static void initializePortB(void)
{
    DDRB = 0;
    SET_BIT(DDRB, DDB1);
    SET_BIT(DDRB, DDB2);
    SET_BIT(DDRB, DDB3);
    SET_BIT(DDRB, DDB5);
}

static void initializePortD(void)
{
    DDRD = 0;
    SET_BIT(DDRD, DDD3);
    SET_BIT(DDRD, DDD5);
    SET_BIT(DDRD, DDD6);
}

static void initializeAnalogInput(void)
{
    ADMUX = 0;
    ADCSRA = 0;
    // reference voltage is vcc (5v?), 10 bit mode, using pin A0
    ADMUX |= (1 << REFS0) | (0 << ADLAR);
    // enable ADC module, set prescaler divisor value to 128
    ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
}

static uint16_t readAnalogInput()
{
    SET_BIT(ADCSRA, ADSC);

    while (GET_BIT(ADCSRA, ADSC) == 1)
    {
        _delay_us(10);
    }

    return ADC;
}
static void initializeDevice8(device *device, volatile uint8_t *pitch, volatile uint8_t *width)
{
    device->pitch.pointerSize = 8;
    device->width.pointerSize = 8;
    device->pitch.pointer.eight = pitch;
    device->width.pointer.eight = width;
}

static void initializeDevice16(device *device, volatile uint16_t *pitch, volatile uint16_t *width)
{
    device->pitch.pointerSize = 16;
    device->width.pointerSize = 16;
    device->pitch.pointer.sixteen = pitch;
    device->width.pointer.sixteen = width;
}

static device* initializeDevices(uint8_t *numDevices)
{
    *numDevices = 3;
    device *devices = malloc((*numDevices) * sizeof(device));

    // timer counter 0
    initializeDevice8(&devices[0], &OCR0A, &OCR0B);
    // timer counter 1
    initializeDevice16(&devices[1], &OCR1A, &OCR1B);
    // timer counter 2
    initializeDevice8(&devices[2], &OCR2A, &OCR2B);

    return devices;
}

static void initializeChannel(channel *channel, device *device)
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

static channel* initializeChannels(uint8_t *numChannels, device *devices)
{
    *numChannels = 3;
    channel *channels = malloc((*numChannels) * sizeof(channel));

    for (int i = 0; i < *numChannels; i++)
    {
        initializeChannel(&channels[i], &devices[i]);
    }
    return channels;
}

static void initializeTrack(track *track, channel *channel, sequence_t *sequence, uint16_t sequenceLength)
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
    initializeTrack(&tracks[0], &channels[0], voiceOne, pgm_read_word(&voiceOneLength));
    initializeTrack(&tracks[1], &channels[1], voiceTwo, pgm_read_word(&voiceTwoLength));
    initializeTrack(&tracks[2], &channels[2], voiceThree, pgm_read_word(&voiceThreeLength));
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

    sequence_t *tSequence = target->sequence;
    uint16_t position = target->sPosition;
    uint8_t code = pgm_read_byte(&tSequence[position]);
    channel *tChannel = target->channel;

    switch (code) 
    {
        case 0:
            // sleep for duration
            target->remainingSleepTime =
                pgm_read_byte(&tSequence[position+1]) * sleepUnit * rhythmUnit;
            target->sPosition = position+2;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            // Set pitches
            // pitches + sleep combo to save space
            target->remainingSleepTime =
                pgm_read_byte(&tSequence[position+code+1]) * sleepUnit * rhythmUnit;
            tChannel->currentPitchCount = code;
            target->sPosition = position + code + 2;
            for (int i = 0; i < code; i++)
            {
                tChannel->currentPitches[i] = pgm_read_word(&tSequence[position+i+1]);
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
            target->channel->currentTone = pgm_read_byte(&tSequence[position+code+1]);
            target->remainingSleepTime =
                pgm_read_byte(&tSequence[position+code+2]) * sleepUnit * rhythmUnit;
            target->sPosition = position + code + 3;
            tChannel->currentPitchCount = code;
            for (int i = 0; i < code; i++)
            {
                tChannel->currentPitches[i] = pgm_read_word(&tSequence[position+i+1]);
            }
            tChannel->nextPitchIndex = 0;
            break;
        case 5:
            // Set "volume" (voltage)
            tChannel->currentTone =
                pgm_read_byte(&tSequence[position+1]);
            target->sPosition = position+2;
            break;
        case 6:
            // Set instrument function
            tChannel->instrument =
                instruments[pgm_read_byte(&tSequence[position+1])];
            target->sPosition = position+2;
            break;
        case 7:
            // Set rhythm unit (tempo)
            rhythmUnit =
                pgm_read_byte(&tSequence[position+1]);
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
                target->sPosition = position-(pgm_read_byte(&tSequence[position+1]));
            }
            break;
        case 9:
            // volume + sleep
            tChannel->currentTone =
                pgm_read_byte(&tSequence[position+1]);
            target->remainingSleepTime =
                pgm_read_byte(&tSequence[position+2]) * sleepUnit * rhythmUnit;
            target->sPosition = position+3;
            break;
        default:
            /*for (int i = 0; i < code; i++)
            {
                SET_BIT(PORTB, 5);
                _delay_ms(500);
                UNSET_BIT(PORTB, 5);
                _delay_ms(500);
            }*/
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

    initializePortB();
    initializePortD();
    initializeAnalogInput();
    initializeTimerCounter0();
    initializeTimerCounter1();
    initializeTimerCounter2();

    startTimerCounter0();
    startTimerCounter1();
    startTimerCounter2();

    // initialize the devices - interfaces to audio emitting hardware
    composition->devices = initializeDevices(&composition->numDevices);
    // initialize the channels - device usage & state management
    composition->channels = initializeChannels(&composition->numChannels, composition->devices);
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
