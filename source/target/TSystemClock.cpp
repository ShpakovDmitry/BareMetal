/*
 *   file: TSystemClock.cpp
 * author: ShpakovDmitry
 *   date: 2020-12-04
 */
#include <cstdint>
#include "TSystemClock.h"
#include "TSystemClockRegisters.h"
#include "TCrystalOscillators.h"

#define START_TASK(reg) ( (reg) = (1) )

static TSystemClockRegisters* const clock = \
                 reinterpret_cast<TSystemClockRegisters*> (CLOCK_BASE_ADDRESS);

static bool isEventStartedHFXO(void) {
   return (clock->EVENTS_HFCLKSTARTED) ? true : false;
}

static bool isEventStartedLFCLK(void) {
    return (clock->EVENTS_LFCLKSTARTED) ? true : false;
}

void TSystemClock::setDebounceTime(TCrystalOscillators::DebounceTime xo) {
    unsigned regVal;
    unsigned const minRegVal = 0x01;
    unsigned const maxRegVal = 0xff;
    
    regVal = xo / 16; // magic because T_debounce = reg_val * 16
    
    // regVal should be in range 0x01..0xff. if not then clip value
    if (regVal < minRegVal) {
        regVal = minRegVal;
    }
    else if (regVal > maxRegVal) {
        regVal = maxRegVal;
    }
    clock->HFXODEBOUNCE = regVal;
}

void TSystemClock::selectHighFreqSource(HighFreqSource src) {
    switch (src) {
        case HighFreqSource::HFXO:
            START_TASK(clock->TASKS_HFCLKSTART);
            while ( isEventStartedHFXO() == false ) {;}
            break;
        case HighFreqSource::HFINT:
            START_TASK(clock->TASKS_HFCLKSTOP);
            // after this MCU clock control system should
            // fallback to internal high frequency oscillator
            break;
        default:    // never will get here
            break;
    }
}

void TSystemClock::selectLowFreqSource(LowFreqSource src) {
    START_TASK(clock->TASKS_LFCLKSTOP);
    while ( isEventStartedLFCLK() == true ) {;}

    switch (src) {
        case LowFreqSource::LFXO:
            clock->LFCLKSRC |= \
                 static_cast<uint32_t> (TSystemClockBitFields::LFCLKSRC::XTAL);
            break;
        case LowFreqSource::LFRC:
            clock->LFCLKSRC &= \
                static_cast<uint32_t> (~(TSystemClockBitFields::LFCLKSRC::RC));
            break;
        case LowFreqSource::SYNTH:
            clock->LFCLKSRC |= \
                static_cast<uint32_t> (TSystemClockBitFields::LFCLKSRC::SYNTH);
            break;
        default:    // should never get here
            break;
    }

    START_TASK(clock->TASKS_LFCLKSTART);
    while ( isEventStartedLFCLK() == false ) {;}
}

TSystemClock::TSystemClock() {
    setDebounceTime(TCrystalOscillators::MAXIMAL);
}
