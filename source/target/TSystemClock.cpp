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
    regVal = xo / 16; // magic because T_debounce = reg_val * 16
    // regVal should be in range 0x01..0xff. if not then clip value
    if (regVal < 0x01) {
        regVal = 0x01;
    }
    else if (regVal > 0xff) {
        regVal = 0xff;
    }
    clock->HFXODEBOUNCE = regVal;
}

void TSystemClock::selectHighFreqSource(HighFreqSource src) {
    switch (src) {
        case HighFreqSource::HFXO:
            START_TASK(clock->TASKS_HFCLKSTART);
            break;
        case HighFreqSource::HFINT:
            START_TASK(clock->TASKS_HFCLKSTOP);
            break;
        default:    // never will get here
            break;
    }
}
