.equ XL, r26
.equ XH, r27
.equ YL, r28
.equ YH, r29
.equ ZL, r30
.equ ZH, r31

.equ REG_FILE_SIZE, 0x20
.equ IO_FILE_SIZE,  0x40
.equ SRAM_SIZE,     0x80
.equ RAMEND,        REG_FILE_SIZE + IO_FILE_SIZE + SRAM_SIZE
.equ RAMSTART,      REG_FILE_SIZE + IO_FILE_SIZE
.equ SPL,           0x3d
