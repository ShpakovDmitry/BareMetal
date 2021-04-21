/*
 *   file: TSystemClock.h
 * author: ShpakovDmitry
 *   date: 2020-12-04
 */

#ifndef T_SYSTEM_CLOCK_H
#define T_SYSTEM_CLOCK_H

#include "TCrystalOscillators.h"

class TSystemClock {
public:
    enum class HighFreqSource { HFINT, HFXO };
    enum class LowFreqSource { LFXO, LFRC, SYNTH };

    void setDebounceTime(TCrystalOscillators::DebounceTime xo = \
                                         TCrystalOscillators::MAXIMAL);
    void selectHighFreqSource(HighFreqSource src);
    void selectLowFreqSource(LowFreqSource src);
    //TODO
    // * add LFRC calibration
    // * interrupt handlers
    //
    
    TSystemClock();
    ~TSystemClock();
private:
};

#endif  // T_SYSTEM_CLOCK_H
