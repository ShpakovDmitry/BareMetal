/*
 *   file: TApplication.h
 * author: ShpakovDmitry
 *   date: 2020-12-1
 */

#ifndef TAPPLICATION_H
#define TAPPLICATION_H

#include <target/TTargetNRF52840.h>

class TApplication {
public:
    void run();
private:
    TTargetNRF52840 _target;
};

#endif  // TAPPLICATION_H
