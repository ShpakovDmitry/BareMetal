.include "attiny2313def.inc"

.section .text,"ax",@progbits

.global _init
_init:
	eor r1, r1			; set r1 reg as zero, this is for convention
	ldi r20, lo8(RAMEND - 1)	; stack setup
	out SPL, r20

clearBss:
	ldi XL, lo8(_sbss)
	ldi XH, hi8(_sbss)
	ldi r20, lo8(_ebss)
	ldi r21, hi8(_ebss)
	rjmp clearBssStart

clearBssLoop:
	st X+, r1

clearBssStart:
	cp  XL, r20
	cpc XH, r21
	brlo clearBssLoop

copyData:				; copy data to RAM
	ldi ZH, hi8(_etext)
	ldi ZL, lo8(_etext)
	ldi XH, hi8(_sdata)
	ldi XL, lo8(_sdata)
	ldi r20, hi8(_edata)
	ldi r21, lo8(_edata)
	rjmp copyDataStart

copyDataLoop:
	lpm r0, Z+
	st X+, r0

copyDataStart:
	cp  XL, r20
	cpc XH, r21
	brlo copyDataLoop
	rcall main			; main is called, here it is entry

infLoop:
	rjmp infLoop			; will be executed whe main returns
