INCLUDE etc/linker/attiny2313.ld

MEMORY {
    FLASH (rx)      : ORIGIN = FLASH_BASE,            LENGTH = FLASH_SIZE
    SRAM  (rw!x)    : ORIGIN = SRAM_BASE + SRAMSTART, LENGTH = SRAM_SIZE
}

SECTIONS {
    . = ORIGIN(FLASH);
    .text ALIGN(2) : {
        KEEP(*(.vectors))
        *(.text)
        *(.text*)
        KEEP(*(.rodata*))
    } > FLASH

        __etext = .;

    __stacktop = ALIGN (ORIGIN(SRAM) + LENGTH(SRAM));
    __data_load = LOADADDR(.data);
    . = ORIGIN(SRAM);

    .data ALIGN(2) : {
        __data_start = .;
        *(.data)
        *(.data*)
        . = ALIGN(2);
        __data_end = .;
    } > SRAM AT > FLASH

    .bss ALIGN(2) (NOLOAD) : {
        __bss_start = .;
        *(.bss)
        *(.bss*)
        . = ALIGN(2);
        __bss_end = .;
        *(.noinit*)
    } > SRAM

    . = ALIGN(2);
    __heap_start = .;

}
