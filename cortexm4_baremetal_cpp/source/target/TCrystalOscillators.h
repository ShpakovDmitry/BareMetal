/*
 *   file: TCrystalOscillators.h
 * author: ShpakovDmitry
 *   date: 2020-12-04
 */

#ifndef T_CRYSTAL_OSCILLATORS_H
#define T_CRYSTAL_OSCILLATORS_H
namespace TCrystalOscillators {
    enum DebounceTime {
    MINIMAL = 16,
    TSX_3225 = 256,
    FA_20H = 256,
    FA_128 = 256,
    NX1212AA = 1024,
    NX1210AB = 1024,
    MAXIMAL = 4080
    };
}

#endif  // T_CRYSTAL_OSCILLATORS_H
