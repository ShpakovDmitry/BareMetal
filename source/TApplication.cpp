/*
 *   file: TApplication.cpp
 * author: ShpakovDmitry
 *   date: 2020-12-1
 */

#include <TApplication.h>

void TApplication::run() {
    while (true) {
        LED_on();
        _target.delay(10);
        LED_off();
        _target.delay(10);
    }
}
