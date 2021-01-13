/*
 *   file: Application.c
 * author: ShpakovDmitry
 *   date: 2020-12-18
 */

#include <application/scheduler/scheduler.h>
#include <target/nRF52840.h>
#include "application.h"

static void Application_initHardware();
static void Application_initScheduler();
static void Application_runScheduler();
static void Application_halt();
static int Application_sendMessage(void);
static int Application_blinkLed1(void);
static int Application_blinkLed2(void);
static int Application_blinkLed3(void);
static int Application_blinkLed4(void);

void Application_run(void) {
    Application_initHardware();
    Application_initScheduler();
    Application_runScheduler();

    // should never get here
    Application_halt();
}

static void Application_initHardware(void) {
    nRF52840_init();
    nRF52840_initLeds();
}

static void Application_initScheduler(void) {
    const Scheduler_Time SEND_MESSAGE_PERIOD = 1000;
    const Scheduler_Time LED1_BLINK_PERIOD = 500;
    const Scheduler_Time LED2_BLINK_PERIOD = 501;
    const Scheduler_Time LED3_BLINK_PERIOD = 502;
    const Scheduler_Time LED4_BLINK_PERIOD = 503;

    Scheduler_addTask(&Application_sendMessage,  SEND_MESSAGE_PERIOD);
    Scheduler_addTask(&Application_blinkLed1,  LED1_BLINK_PERIOD);
    Scheduler_addTask(&Application_blinkLed2,  LED2_BLINK_PERIOD);
    Scheduler_addTask(&Application_blinkLed3,  LED3_BLINK_PERIOD);
    Scheduler_addTask(&Application_blinkLed4,  LED4_BLINK_PERIOD);
}

static void Application_runScheduler(void) {
    Scheduler_run();
}

static void Application_halt() {
    while (1) {
        ;
    }
}

int Application_sendMessage(void) {
    nRF52840_puts("Hello from nRF52840-DK\n");
    return 0;
}

static int Application_blinkLed1(void) {
    nRF52840_invertLed(LED1);
    return 0;
}

static int Application_blinkLed2(void) {
    nRF52840_invertLed(LED2);
    return 0;
}

static int Application_blinkLed3(void) {
    nRF52840_invertLed(LED3);
    return 0;
}

static int Application_blinkLed4(void) {
    nRF52840_invertLed(LED4);
    return 0;
}

