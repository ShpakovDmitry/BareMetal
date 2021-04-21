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
