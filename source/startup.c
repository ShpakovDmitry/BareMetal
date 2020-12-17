extern __UINT16_TYPE__ __data_start;
extern __UINT16_TYPE__ __data_end;
extern __UINT16_TYPE__ __data_load;
extern __UINT16_TYPE__ __bss_start;
extern __UINT16_TYPE__ __bss_end;
extern __UINT16_TYPE__ __heap_start;

extern void main(void);
extern void __stop();

void copyDataSection(void) {
    __UINT16_TYPE__ *src, *dst;
    src = &__data_load;
    dst = &__data_start;
    while (dst < &__data_end) {
        *(dst++) = *(src++);
    }
}

void copyBssSection(void) {
    __UINT16_TYPE__ *src;
    src = &__bss_start;
    while (src < &__bss_end) {
        *(src++) = 0;
    }
}

void fillHeap(__UINT16_TYPE__ fillVal) {
    __UINT16_TYPE__ *dst, *spl;
    dst = &__heap_start;
    __asm__ volatile(
            "in  %A0, 0x3d\n" "\n\t"
            "ldi %B0, 0x00\n" "\n\t"
            : "=r" (spl));
    while (dst < spl) {
        *(dst++) = fillVal;
    }
}

 void _initCRT() {
    copyDataSection();
    copyBssSection();
    fillHeap(0xC0DE);
    main();
    __stop();
}
