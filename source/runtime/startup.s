.include "attiny2313def.inc"

.section .text,"ax",@progbits

.global _init
_init:
	; stack setup
	ldi r20, lo8(RAMEND - 1)
	out SPL, r20

	; copy data to RAM
	ldi ZH, hi8(_etext)
	ldi ZL, lo8(_etext)
	ldi XH, hi8(_sdata)
	ldi XL, lo8(_sdata)

	ldi r20, hi8(_edata)
	ldi r21, lo8(_edata)

	rjmp copyCompare

copyStore:
	lpm r0, Z+
	st X+, r0

copyCompare:
	cp XH, r20
	cpc XL, r21
	brlo copyStore
	rcall main	; main is called, here it is entry

infLoop:
	rjmp infLoop	; will be executed whe main returns
