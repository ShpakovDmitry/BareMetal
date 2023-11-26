/*
 *   file: TSysTick.cpp
 * author: ShpakovDmitry
 *   date: 2020-12-14
 */

#include <cstdint>
#include "TSysTick.h"

static const uint32_t SYSTICK_BASE_ADDRESS = 0xE000E000;
static const uint32_t SYSTEM_FREQ = 64'000'000;
static const uint32_t MAX_SYSTICK_REG_VAL = 0x00FFFFFF;

typedef volatile struct __attribute__((packed)) {
    uint32_t reserved0[4];  /* 0x000 - 0x00C reserved*/
    uint32_t SYST_CSR;      /* 0x010 - SysTick control and status register */
    uint32_t SYST_RVR;      /* 0x014 - SysTick reload value register */
    uint32_t SYST_CVR;      /* 0x018 - SysTick current value register */
    uint32_t SYST_CALIB;    /* 0x01C - SysTick calibration value register */
} TSysTickRegister;

namespace TSysTickBits {
    namespace SYST_CSR {
        enum {
            COUNTFLAG = 16,
            CLKSOURCE = 2,
            TICKINT = 1,
            ENABLE = 0
        };
    }
    namespace SYST_RVR {
        enum {
            RELOAD = 0b0000'1111'1111'1111
        };
    }
    namespace SYST_CVR {
        enum {
            CURRENT = 0b0000'1111'1111'1111
        };
    }
    namespace SYST_CALIB {
        enum {
            NOREF = 31,
            SKEW = 30,
            TENMS = 0b0000'1111'1111'1111
        };
    }
}

static TSysTickRegister* const sysTick = reinterpret_cast<TSysTickRegister*>(SYSTICK_BASE_ADDRESS);


bool TSysTick::setReloadPeriod(uint32_t periodMicroSec) {
    constexpr static uint32_t TICKS_PER_US = (SYSTEM_FREQ / 1'000'000);
    constexpr static uint32_t MAX_SYSTICK_PERIOD_US = 
        (MAX_SYSTICK_REG_VAL + 1) / TICKS_PER_US;
    
    if ( periodMicroSec == 0 || periodMicroSec > MAX_SYSTICK_PERIOD_US ) {
        return false;
    }

    uint32_t reloadRegisterVal;
    reloadRegisterVal = periodMicroSec * TICKS_PER_US - 1;

    sysTick->SYST_RVR = reloadRegisterVal;

    return true;
}

bool TSysTick::setCurrentValue(uint32_t value) {
    if ( value > MAX_SYSTICK_REG_VAL ) {
        return false;
    }

    sysTick->SYST_CVR = value;

    return true;
}

void TSysTick::enable(void) {
    sysTick->SYST_CSR |= 
        static_cast<uint32_t>((1 << TSysTickBits::SYST_CSR::ENABLE));
}

void TSysTick::disable(void) {
    sysTick->SYST_CSR &= 
        ~static_cast<uint32_t>((1 << TSysTickBits::SYST_CSR::ENABLE));
}

bool TSysTick::isEnabled(void) {
    return (sysTick->SYST_CSR & static_cast<uint32_t>((1 << TSysTickBits::SYST_CSR::ENABLE))) ? true : false;
}

void TSysTick::enableInterrupt(void) {
    sysTick->SYST_CSR |=
        static_cast<uint32_t>( (1 << TSysTickBits::SYST_CSR::TICKINT) );
}

void TSysTick::disableInterrupt(void) {
    sysTick->SYST_CSR &= 
        ~static_cast<uint32_t>((1 << TSysTickBits::SYST_CSR::TICKINT));
}

bool TSysTick::isInterruptEnabled(void) {
    return (sysTick->SYST_CSR & static_cast<uint32_t>((1 << TSysTickBits::SYST_CSR::TICKINT))) ? true : false;
}

static void dummy(void) {
    ;
}

void (* handlerCallback)(void) = dummy;

void TSysTick::setInterruptCallback(void (*cb)(void)) {
    if (cb == nullptr) {
        return;
    }
    handlerCallback = cb;
}

__attribute__((isr)) void SYSTICK_Handler(void) {
    sysTick->SYST_CSR &= 
        ~static_cast<uint32_t>((1 << TSysTickBits::SYST_CSR::TICKINT));
    
    handlerCallback();
    
    sysTick->SYST_CSR |=
        static_cast<uint32_t>( (1 << TSysTickBits::SYST_CSR::TICKINT) );
}
