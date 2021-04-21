/*
 *   file: TNvic.h
 * author: ShpakovDmitry
 *   date: 2020-12-07
 */

#ifndef T_NVIC_H
#define T_NVIC_H

#include <cstdint>
#include "TNvicIrq.h"

class TNvic {
public:
    void enableGlobalIrq(void);
    void disableGlobalIrq(void);
    void enable(TNvicIrq::TNvicIrq irq);
    void disable(TNvicIrq::TNvicIrq irq);
    void setPending(TNvicIrq::TNvicIrq irq);
    void clearPending(TNvicIrq::TNvicIrq irq);
    bool isPending(TNvicIrq::TNvicIrq irq);
    void setPriority(TNvicIrq::TNvicIrq irq, uint8_t priority);
private:
};

#endif  // T_NVIC_H
