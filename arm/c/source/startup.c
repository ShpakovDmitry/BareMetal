/*
 *   file: startup.c
 * author: ShpakovDmitry
 *   date: 2020-08-20
 */

typedef void (*funcPtr)();

extern __UINT32_TYPE__ __data_start;
extern __UINT32_TYPE__ __data_end;
extern __UINT32_TYPE__ __data_load;
extern __UINT32_TYPE__ __bss_start;
extern __UINT32_TYPE__ __bss_end;
extern __UINT32_TYPE__ __heap_start;

extern void main(void);
extern void fillHeap(__UINT32_TYPE__ fillVal);

extern funcPtr __preinit_array_start[];
extern funcPtr __preinit_array_end[];
extern funcPtr __init_array_start[];
extern funcPtr __init_array_end[];
extern funcPtr __fini_array_start[];
extern funcPtr __fini_array_end[];

void copyDataSection(void) {
    __UINT32_TYPE__ *src, *dst;
    src = &__data_load;
    dst = &__data_start;
    while (dst < &__data_end) {
        *(dst++) = *(src++);
    }
}

void copyBssSection(void) {
    __UINT32_TYPE__ *src;
    src = &__bss_start;
    while (src < &__bss_end) {
        *(src++) = 0;
    }
}

void callInitArray(void) {
    funcPtr* array = __preinit_array_start;
    while (array < __preinit_array_end) {
        (*array)();
        array++;
    }
    array = __init_array_start;
    while (array < __init_array_end) {
        (*array)();
        array++;
    }
}

void callFiniArray(void) {
    funcPtr* array = __fini_array_start;
    while (array < __fini_array_end) {
        (*array)();
        array++;
    }
}

void fillHeap(__UINT32_TYPE__ fillVal) {
    __UINT32_TYPE__ *dst, *mspReg;
    dst = &__heap_start;
    __asm__("mrs %0, msp\n" : "=r" (mspReg));
    while (dst < mspReg) {
        *(dst++) = fillVal;
    }
}

void RESET_Handler() {
    copyDataSection();
    copyBssSection();
    fillHeap(0xDEADC0DE);
    callInitArray();
    main();
    callFiniArray();
    while (1) {
        ;
    }
}
