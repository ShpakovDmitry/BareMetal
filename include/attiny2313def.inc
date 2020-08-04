.equ XH, r26
.equ XL, r27
.equ YH, r28
.equ YL, r29
.equ ZH, r30
.equ ZL, r31

.equ REG_FILE_SIZE, 0x20
.equ IO_FILE_SIZE,  0x40
.equ SRAM_SIZE,     0x80
.equ RAMEND,        REG_FILE_SIZE + IO_FILE_SIZE + SRAM_SIZE
.equ RAMSTART,      REG_FILE_SIZE + IO_FILE_SIZE
.equ SPL,           0x3d
