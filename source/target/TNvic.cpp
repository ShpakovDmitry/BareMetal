/*
 *   file: TNvic.cpp
 * author: ShpakovDmitry
 *   date: 2020-12-07
 */

#include <cstdint>
#include "TNvic.h"
#include "TNvicRegisters.h"

static TNvicRegisters* const nvic = \
                            reinterpret_cast<TNvicRegisters* >(NVIC_BASE_ADDR);

#define SET_BIT_HI(reg, bit) ( (reg) |= (1 << (bit)) )
#define GET_BIT(reg, bit)    ( (reg)  & (1 << (bit)) )

void TNvic::enableGlobalIrq(void) {
    __asm__("CPSIE I");
}

void TNvic::disableGlobalIrq(void) {
    __asm__("CPSID I");
}

static const uint8_t BITS_IN_ARM_WORD = 32;

static uint8_t getRegPosition(TNvicIrq::TNvicIrq irq) {
    return static_cast<uint8_t>(irq) / BITS_IN_ARM_WORD;
}

static uint8_t getBitPosition(TNvicIrq::TNvicIrq irq) {
    return static_cast<uint8_t>(irq) % BITS_IN_ARM_WORD;
}

static const uint8_t MSB_PRIORITY = 3;    // in IPR 3 MSB matters
static const unsigned BITS_IN_BYTE = 8;
static const uint8_t MAX_PRIORITY = 7;
static const uint8_t MIN_PRIORITY = 0;

static bool isCorrectPriority(uint8_t priority) {
    bool res = false;

    if ( priority <= MAX_PRIORITY ) {
        res = true;
    }

    return res;
}

static uint8_t getPriorityValue(uint8_t priority) {
    return ( priority << (BITS_IN_BYTE - MSB_PRIORITY) );
}

void TNvic::enable(TNvicIrq::TNvicIrq irq) {
    uint8_t irqReg, irqBit;

    irqReg = getRegPosition(irq);
    irqBit = getBitPosition(irq);

    SET_BIT_HI(nvic->NVIC_ISER[irqReg], irqBit);
}

void TNvic::disable(TNvicIrq::TNvicIrq irq) {
    uint8_t irqReg, irqBit;

    irqReg = getRegPosition(irq);
    irqBit = getBitPosition(irq);

    SET_BIT_HI(nvic->NVIC_ICER[irqReg], irqBit);
}

void TNvic::setPending(TNvicIrq::TNvicIrq irq) {
    uint8_t irqReg, irqBit;

    irqReg = getRegPosition(irq);
    irqBit = getBitPosition(irq);

    SET_BIT_HI(nvic->NVIC_ISPR[irqReg], irqBit);
}

void TNvic::clearPending(TNvicIrq::TNvicIrq irq) {
    uint8_t irqReg, irqBit;

    irqReg = getRegPosition(irq);
    irqBit = getBitPosition(irq);

    SET_BIT_HI(nvic->NVIC_ICPR[irqReg], irqBit);
}

bool TNvic::isPending(TNvicIrq::TNvicIrq irq) {
    bool res = false;
    uint8_t irqReg, irqBit;
    
    irqReg = getRegPosition(irq);
    irqBit = getBitPosition(irq);
    
    if ( GET_BIT(nvic->NVIC_IABR[irqReg], irqBit) ) {
        res = true;
    }

    return res;
}

void TNvic::setPriority(TNvicIrq::TNvicIrq irq, uint8_t priority) {
    if ( !isCorrectPriority(priority) ) {
        return;
    }

    uint8_t priorityVal;
    priorityVal = getPriorityValue(priority);

    nvic->NVIC_IPR[irq] = priorityVal;
}
