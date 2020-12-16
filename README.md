### AVR25BareMetal
AVR25 architecture ATtiny2313 microcontroller bare metal bring-up without
the uses of any libraries.
All source code build using GNU GCC toolchain.

#### Cloning the project
To clone the project to your local directory, run the following:
```bash
git clone https://github.com/ShpakovDmitry/AVR25BareMetal.git
```

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
###### Zero `r1` register.
When the C source code is compiled into microcontroller assembly instruction
listing by compiler, there is convention about registers usage. Register `r0`
is always a temporary register and register `r1` is always zero. Compiler
will blindly rely on this assumptions.\
To zero `r1` register exclusive OR instruction could be used:
```asm
eor r1, r1  ;set r1 reg as zero, this is for convention
```




###### Interrupt Vector Table
Also starting from address `0x0000` interrupt vector table is located.

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
