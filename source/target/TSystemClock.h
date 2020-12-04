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
    enum class LowFreqSource { LFXO, LFRC, SYNT };

    void setDebounceTime(TCrystalOscillatorDebounce xo = \
                                         TCrystalOscillatorDebounce::MAXIMAL);
    void selectHighFreqSource(HighFreqSource src);
    bool isStartedHFXO();
    void selectLowFreqSource(LowFreqSource src);
    
    TSystemClock();
    ~TSystemClock();

private:

};

#endif  // T_SYSTEM_CLOCK_H
