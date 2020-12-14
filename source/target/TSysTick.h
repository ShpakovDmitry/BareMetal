/*
 *   file: TSysTick.h
 * author: ShpakovDmitry
 *   date: 2020-12-14
 */

#ifndef T_SYSTICK_H
#define T_SYSTICK_H

#include <cstdint>

class TSysTick {
public:
    bool setReloadPeriod(uint32_t periodMicroSec);
    bool setCurrentValue(uint32_t value);
    void enable(void);
    void disable(void);
private:
};

#endif  // T_SYSTICK_H
