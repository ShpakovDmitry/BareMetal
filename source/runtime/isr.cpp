/*
 *   file: isr.cpp
 * author: ShpakovDmitry
 *   date: 2020-11-27
 */

#include <cstdint>


extern uint32_t __stacktop;

__attribute__((section(".stack"), used)) uint32_t *__stack_init = &__stacktop;

typedef void (*funcPtr)();

extern "C" void __stop(void) {
    while (true) {
        ;
    }
}

void RESET_Handler(void);       __attribute__((weak, alias("__stop")));
void NMI_Handler(void);         __attribute__((weak, alias("__stop")));
void HARDFAULT_Handler(void);   __attribute__((weak, alias("__stop")));
void MEMMANAGE_Handler(void);   __attribute__((weak, alias("__stop")));
void BUSFAULT_Handler(void);    __attribute__((weak, alias("__stop")));
void HARDFAULT_Handler(void);   __attribute__((weak, alias("__stop")));
void USAGEFAULT_Handler(void);  __attribute__((weak, alias("__stop")));
void SVCALL_Handler(void);      __attribute__((weak, alias("__stop")));
void PENDSV_Handler(void);      __attribute__((weak, alias("__stop")));
void SYSTICK_Handler(void);     __attribute__((weak, alias("__stop")));

void DUMMY_Handler(void);       __attribute__((weak, alias("__stop")));

__attribute__((section(".exception_vectors"), used)) funcPtr __exceptionVectors[] = {
    RESET_Handler,
    NMI_Handler,
    HARDFAULT_Handler,
    MEMMANAGE_Handler,
    BUSFAULT_Handler,
    USAGEFAULT_Handler,
    DUMMY_Handler,
    DUMMY_Handler,
    DUMMY_Handler,
    DUMMY_Handler,
    SVCALL_Handler,
    DUMMY_Handler,
    DUMMY_Handler,
    PENDSV_Handler,
    SYSTICK_Handler
}
