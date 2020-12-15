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
