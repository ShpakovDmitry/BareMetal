/*
 *   file: TSystemClock.cpp
 * author: ShpakovDmitry
 *   date: 2020-12-04
 */

#include "TSystemClock.h"
#include "TSystemClockRegisters.h"
#include "TCrystalOscillators.h"

#define START_TASK(reg) ( (reg) = (1) )

static TSystemClockRegisters* const clock = \
                 reinterpret_cast<TSystemClockRegisters*> (CLOCK_BASE_ADDRESS);

void TSystemClock::setDebounceTime(TCrystalOscillatorDebounce xo) {
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
            while ( isStartedHFXO() == false ) {;}
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

bool TSystemClock::isStartedHFXO() {
   return (clock->EVENTS_HFCLKSTARTED) ? true : false;
}
