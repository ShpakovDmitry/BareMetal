.section INT_VECTOR, "x"
.global _Reset

_Reset:
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	rjmp ResetHandler
	
ResetHandler:
	eor r1, r1
	out 0x3f, r1
	ldi r28, 0xdf
	out 0x3d, r28
	rcall entry
loop:
	rjmp loop
