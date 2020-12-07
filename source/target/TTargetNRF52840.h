/*
 *   file: TTargetNRF52840.h
 * author: ShpakovDmitry
 *   date: 2020-12-04
 */

#ifndef T_TARGET_NRF52840_H
#define T_TARGET_NRF52840_H

#include "TSystemClock.h"
#include "TNvic.h"

class TTargetNRF52840 {
public:
    void reset();
    void initSystemClock();
    TTargetNRF52840();
    ~TTargetNRF52840() { reset(); }
private:
    TSystemClock _systemClock;
    TNvic _nvic;
};

#endif  // T_TARGET_NRF52840_H
