#include <stdint.h>
#include <stdbool.h>

extern uint16_t __data_start;
extern uint16_t __data_end;
extern uint16_t __data_load;
extern uint16_t __bss_start;
extern uint16_t __bss_end;
extern uint16_t __heap_start;

extern void main(void);

void copyDataSection(void) {
    uint16_t *src, *dst;
    src = &__data_load;
    dst = &__data_start;
    while (dst < &__data_end) {
        *(dst++) = *(src++);
    }
}

void copyBssSection(void) {
    uint16_t *src;
    src = &__bss_start;
    while (src < &__bss_end) {
        *(src++) = 0;
    }
}

void fillHeap(uint16_t fillVal) {
    uint16_t *dst, *spl;
    dst = &__heap_start;
    __asm__("in %0, 0x3d\n" : "=r" (spl));
    while (dst < spl) {
        *(dst++) = fillVal;
    }
}

 void _initCRT() {
    copyDataSection();
    copyBssSection();
    fillHeap(0xC0DE);
    main();
    while (true) {
        ;
    }
}
