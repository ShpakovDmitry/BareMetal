.include "attiny2313def.inc"

;
; using weak symbol to handle ISR overwriting
;
.macro	vector name
.weak 	name
.set	name, ISR_dummy
  rjmp 	name
.endm

.section .vectors,"ax",@progbits
.global __vectors
__vectors:
	rjmp ISR_reset
	vector __vector_2
	vector __vector_3
	vector __vector_4
	vector __vector_5
	vector __vector_6
	vector __vector_7
	vector __vector_8
	vector __vector_9
	vector __vector_10
	vector __vector_11
	vector __vector_12
	vector __vector_13
	vector __vector_14
	vector __vector_15
	vector __vector_16
	vector __vector_17
	vector __vector_18
	vector __vector_19

.section .text,"ax",@progbits
.global ISR_dummy
ISR_dummy:
	reti

.global ISR_reset
ISR_reset:
	rjmp _init
