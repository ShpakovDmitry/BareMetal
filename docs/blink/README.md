### Blink
Led blink is done using delay loop and GPIO registers.

1. First of all we need define microcontroller `GPIO` direction set/clear
registers and `GPIO` output set/clear registers:
```c
/* GPIO_0 direction set/clear registers; */
#define GPIO_0_DIRSET (*(volatile uint32_t* )0x50000518u)
#define GPIO_0_DIRCLR (*(volatile uint32_t* )0x5000051Cu)

/* GPIO_0 output set/clear registers */
#define GPIO_0_OUTSET (*(volatile uint32_t* )0x50000508u)
#define GPIO_0_OUTCLR (*(volatile uint32_t* )0x5000050Cu)
```

2. Choose `LED1` at development kit, which is at `GPIO` port 0 and pin 13:
```c
#define LED_PIN 13
```

3. In `main()` set LED pin as output and loop `LED` toggle forever:
```c
int main(void) {
    /* LED1 on DK is at GPIO port 0 and pin 13 */
    /* set this port as output */
    GPIO_0_DIRSET = 1 << LED_PIN;
    
    while (1) {
        GPIO_0_OUTSET = 1 << LED_PIN;
        delay_loop();
        GPIO_0_OUTCLR = 1 << LED_PIN;
        delay_loop();
    }
    
    return 0;
}
```

4. Delay loop is defined as for lopp which does nothing. Note `volatile` keyword
is used to do not let compiler to optimize this code section:
```c
void delay_loop() {
    for (volatile uint32_t i = 0; i < 0x00100000; ++i ) {
        ;
    }
}
```
The digit `0x00100000` is choosed experimentally to make blink visible. Because
if it will be very small then `LED` toggle rate will be to high to see it by
eye.
