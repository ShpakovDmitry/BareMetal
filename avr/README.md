### AVR25BareMetal
AVR25 architecture ATtiny2313 microcontroller bare metal bring-up without
the use of any libraries.
All source code build using GNU GCC toolchain.

#### Toolchain setup
To build the project following tools required:
* avr-gcc
* avr-binutils
* avr-libc
* make
* avrdude

To get them, run the following on your Linux machine:
```bash
# Ubuntu, Debian
sudo apt-get update
sudo apt-get install avr-gcc avr-binutils avr-libc avrdude
# Arch
sudo pacman -S avr-gcc avr-binutils avr-libc avrdude
```

#### Project build
To build project run the following:
```bash
make
```
To clean build files run:
```bash
make clean
```

#### Description.
##### Microcontroller startup.
When the AVR microcontroller leaves `RESET` state, it starts to execute
instruction at address `0x0000`.
Typically this is relative jump instruction `rjmp` to microcontroller init
procedure. In init routine following should be done:
* zero `r1` register. This is for compiler convention (valid for GCC, should be
checked on other compilers)
* load stack address to stack pointer register `SPL`.
* copy `.data` section from `FLASH` to `SRAM`
* zero `.bss` section in `SRAM`
* optionally fill `heap` with init value
* call `main()` routine

Microcontroller startup code entry is located at `source/vectors.s` file. Here
is assembly listing:
```asm
.section .vectors,"ax",@progbits
.global __vectors
__vectors:
    rjmp RESET_Handler
    rjmp INT0_Handler
    rjmp INT1_Handler
    rjmp TIMER1_CAPT_Handler
    rjmp TIMER1_COMPA_Handler
    rjmp TIMER1_OVF_Handler
    rjmp TIMER0_OVF_Handler
    rjmp USART0_RX_Handler
    rjmp USART0_UDRE_Handler
    rjmp USART0_TX_Handler
    rjmp ANALOG_COMP_Handler
    rjmp PCINT_Handler
    rjmp TIMER1_COMPB_Handler
    rjmp TIMER0_COMPA_Handler
    rjmp TIMER0_COMPB_Handler
    rjmp USI_START_Handler
    rjmp USI_OVF_Handler
    rjmp EE_READY_Handler
    rjmp WDT_OVF_Handler


.section .text,"ax",@progbits
.equ SPL, 0x3d                      ;stack pointer register low byte
.global RESET_Handler
RESET_Handler:
    eor r1, r1                      ;set r1 reg as zero, this is for convention
    ldi r20, lo8(__stacktop - 1)    ;stack setup
    out SPL, r20
    rjmp _initCRT
```
Here are located two sections:
* `.vectors` - interrupt vector table located at `0x0000` in `FLASH` memory.
* `.text` - executable code section which goes to `FLASH` memory.

###### Zero `r1` register.
When the C source code is compiled into microcontroller assembly instruction
listing by compiler, there is convention about registers usage. Register `r0`
is always a temporary register and register `r1` is always zero. Compiler
will blindly rely on this assumptions.\
To zero `r1` register exclusive OR instruction could be used:
```asm
eor r1, r1  ;set r1 reg as zero, this is for convention
```
###### Load stack address to stack pointer register `SPL`.
When any routine is called by `rcall, icall` instruction or return from routine
with `ret, reti` instruction is made, stack is used to store/restore return
address. So, it is crucial to set the initial stack poiter value prior to any
such instruction call. Typically stack pointer points to `SRAM` end. This is 
done in the following way:
```asm
ldi r20, lo8(__stacktop - 1)    ;stack setup
out SPL, r20
```
Here `__stacktop` is defined in linker file(see below). Minus one because
stack push is post-decrement.

When stack is set it is possible to call any C function.
```asm
rjmp _initCRT   ; runtime init written in C
```

The next initialization actions are written in C and are located in
`source/startup.c` file. Here is source code:
```c
#include <stdint.h>
#include <stdbool.h>

extern uint16_t __data_start;
extern uint16_t __data_end;
extern uint16_t __data_load;
extern uint16_t __bss_start;
extern uint16_t __bss_end;
extern uint16_t __heap_start;

extern void main(void);
extern void __stop();

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
```

And so after executing instruction
```asm
rjmp _initCRT   ; runtime init written in C
```
is called
```c
void _initCRT() {
   copyDataSection();
   copyBssSection();
   fillHeap(0xC0DE);
   main();
   __stop();
}
```

###### Copy `.data` section from `FLASH` to `SRAM`
Because of microcontroller **Harward** architecture( data and code are
separated ) it is needed to copy initialized data from `FLASH` to `SRAM`.
Because when we use, for example:
```c
unsigned ourVar = 42;                   // at .data section

void main() {
    static unsigned myVar = 0xC0DE;     // at .data section
}

// do smth with myVar
```
we need to somehow store `42` value in microcontroller, keeping in mind that
after microcontroller reset usually there is a garbage in `SRAM`. This is
done by writing this value to `FLASH` memory and then copying it to `SRAM`
memory.
First of all we need to know the load address of `.data` section in `FLASH`,
it means where it is located in `FLASH` physical address. Then we need to
know what is `.data` section address in `SRAM`. All this addresses are defined
in linker script file(see below). When we know these addresses, we simply copy
data from `FLASH` to `SRAM`
```c
void copyDataSection(void) {
    uint16_t *src, *dst;
    src = &__data_load;
    dst = &__data_start;
    while (dst < &__data_end) {
        *(dst++) = *(src++);
    }
}
```
###### Zero `.bss` section in `SRAM`
Similarly as with `.data` section we do with `.bss` section. The difference is
that in `.bss` section always are located data initialized with zero and
uninitialized global data,i.e.:
```c
unsigned ourVar;            // uninitialized global variable;

void main() {
    static unsigned myVar;  // static data always is initialized with 0
}
```
The zeroing process is do in the following way:
```c
void copyBssSection(void) {
    uint16_t *src;
    src = &__bss_start;
    while (src < &__bss_end) {
        *(src++) = 0;
    }
}
```
The `__bss_start` and `__bss_end` are defined in linker script file(see below).

###### Optionally fill `heap` with init value
This is optional and may serve several purposes. One of them could be for
dynamic memory allocation purposes, the other could be for debug purposes to
detect stack overflow conditions.\
The function:
```c
void fillHeap(uint16_t fillVal) {
    uint16_t *dst, *spl;
    dst = &__heap_start;
    __asm__ volatile(
            "in  %A0, 0x3d\n" "\n\t"
            "ldi %B0, 0x00\n" "\n\t"
            : "=r" (spl));
    while (dst < spl) {
        *(dst++) = fillVal;
    }
}
```
does the following:
* get the heap start address(heap starts after `.data` and `.bss` sections).
* get stack top address by reading stack pointer register `SPL (0x3d)`.
* fill `SRAM` from `__heap_start` to stack top with any value. Here `0xC0DE` is
used.

When debbuging for stack overflow memory dump could be the following:
```diff
some data used in source code                      unused data
            |                                            |
      +-----+-----+--------------------------------------+
      |           |                                      |
0x0060 12 34 23 A3 DE C0 DE C0 DE C0 DE C0 DE C0 DE C0   |
0x0070 DE C0 DE C0 DE C0 DE C0 DE C0 DE C0 DE C0 DE C0   |
0x0080 DE C0 DE C0 DE C0 DE C0 DE C0 DE C0 DE C0 DE C0   |
......................................................   |
0x00D0 DE C0 DE C0 DE C0 DE C0 DE C0 DE C0 00 42 00 1A
                                          |           |
                                          +-----------+
                                                |
                                    data stored on stack
                              (return addresses or stack data)

+ NO STACK AND DATA COLLISION!
```
```diff
some data used in source code                      
            |                                           
      +-----+-------------------->>----------------------+
      |                                                  |
0x0060 12 34 23 A3 34 76 64 50 98 23 87 63 09 40 85 40   |
0x0070 23 48 75 23 27 29 28 59 87 59 21 45 81 34 19 23   |
0x0080 28 45 12 59 83 45 72 18 32 58 26 51 64 52 37 63   |
......................................................   |
0x00D0 12 73 58 32 45 12 34 41 73 51 27 44 00 42 00 1A
                                                      |
                                        <-------------+
                                                |
                                    data stored on stack
                              (return addresses or stack data)

- POSSIBLE STACK AND DATA COLLISION!
```
Possible stack collision could be detected by looking heap fill value,
and if there is no fill pattern found then this may indicate to stack
and data collision. This condition may lead to unpredictable microcontroller
functioning.

###### Call `main()` routine
Here is simple call `main()` routine declared as follows:
```c
void main(void);
```

###### Summary
So, summary this looks the following:
```c
void _initCRT() {
    copyDataSection();
    copyBssSection();
    fillHeap(0xC0DE);
    main();
    __stop();
}
```
After `main()`, there is `__stop()`, which is infinite loop, because if
`main()` return we do not want that microcontroller executes any instruction
after `main()`.

###### Interrupt Vector Table
In `source/isr.c` file there are declared interrupt service routines (ISR)
with weak reference to `__stop()` routine.

```c
#include <stdbool.h>

void __stop(void)  {
    while (true) {;}
}

void INT0_Handler(void)           __attribute__((weak, alias("__stop")));
void INT1_Handler(void)           __attribute__((weak, alias("__stop")));
void TIMER1_CAPT_Handler(void)    __attribute__((weak, alias("__stop")));
void TIMER1_COMPA_Handler(void)   __attribute__((weak, alias("__stop")));
void TIMER1_OVF_Handler(void)     __attribute__((weak, alias("__stop")));
void TIMER0_OVF_Handler(void)     __attribute__((weak, alias("__stop")));
void USART0_RX_Handler(void)      __attribute__((weak, alias("__stop")));
void USART0_UDRE_Handler(void)    __attribute__((weak, alias("__stop")));
void USART0_TX_Handler(void)      __attribute__((weak, alias("__stop")));
void ANALOG_COMP_Handler(void)    __attribute__((weak, alias("__stop")));
void PCINT_Handler(void)          __attribute__((weak, alias("__stop")));
void TIMER1_COMPB_Handler(void)   __attribute__((weak, alias("__stop")));
void TIMER0_COMPA_Handler(void)   __attribute__((weak, alias("__stop")));
void TIMER0_COMPB_Handler(void)   __attribute__((weak, alias("__stop")));
void USI_START_Handler(void)      __attribute__((weak, alias("__stop")));
void USI_OVF_Handler(void)        __attribute__((weak, alias("__stop")));
void EE_READY_Handler(void)       __attribute__((weak, alias("__stop")));
void WDT_OVF_Handler(void)        __attribute__((weak, alias("__stop")));
```

Thats means that if no ISR handler
defined in any translation unit then `__stop()` routine will be used instead.
All ATtiny2313 ISR vectors are the following:

| Vector # | Address | Source | Definition                                    |
|----------|---------|--------|-----------------------------------------------|
|   1      | 0x0000  |RESET       |External Pin, Power-on, Brown-out, Watchdog|
|   2      | 0x0001  |INT0        |External interrupt Request 0               |
|   3      | 0x0002  |INT1        |External interrupt Request 1               |
|   4      | 0x0003  |TIMER1 CAPT |Timer/Counter1 Capture Event               |
|   5      | 0x0004  |TIMER1 COMPA|Timer/Counter1 Compare Match A             |
|   6      | 0x0005  |TIMER1 OVF  |Timer/Counter1 Overflow                    |
|   7      | 0x0006  |TIMER0 OVF  |Timer/Counter0 Overflow                    |
|   8      | 0x0007  |USART0 RX   |USART0 RX Complete                         |
|   9      | 0x0008  |USART0 UDRE |USART0 Data Register Empty                 |
|   10     | 0x0009  |USART0 TX   |USART0 TX Complete                         |
|   11     | 0x000A  |ANALOG COMP |Analog Comparator                          |
|   12     | 0x000B  |PCINT       |Pin Change Interrupt                       |
|   13     | 0x000C  |TIMER1 COMPB|Timer/Counter1 Compare Match B             |
|   14     | 0x000D  |TIMER0 COMPA|Timer/Counter0 Compare Match A             |
|   15     | 0x000E  |TIMER0 COMPB|Timer/Counter0 Compare Match B             |
|   16     | 0x000F  |USI START   |USI Start Condition                        |
|   17     | 0x0010  |USI OVF     |USI Overflow                               |
|   18     | 0x0011  |EE READY    |EEPROM Ready                               |
|   19     | 0x0012  |WDT OVF     |Watchdog Timer Overflow                    |

##### Linker script file
The purpose of the linker is to take object files generated by compiler and
combine(link) them to another object file which could be dumped to
microcontroller firmware. The linker script used is shown below.
```ld
FLASH_BASE = 0x00000000;
SRAM_BASE  = 0x00800000;

REG_FILE_SIZE = 0x20;
IO_FILE_SIZE  = 0x40;
SRAM_SIZE     = 0x80;
FLASH_SIZE    = 2K;

SRAMEND   = REG_FILE_SIZE + IO_FILE_SIZE + SRAM_SIZE;
SRAMSTART = REG_FILE_SIZE + IO_FILE_SIZE;

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
```
