#include "clockConfig.h"
#include "stm32f091xc.h"
#include "timer.h"
#include "uart.h"

const uint32_t blink_fast = 100000;
const uint32_t blink_slow = 400000;
volatile uint32_t blink_curr = blink_fast;

/*
Pins:
 PA5: LED (out, no AF)
 PA6: flag slow/fast (out, no AF)
 PA8: TIM1_CH1 out (AF2)
 PA9: USART1_TX (AF1)
PA10: USART1_RX (AF1)
*/

int main(void)
{
    clockConfig();
    uart_init();
    timer_init();

    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // RCC ON
    GPIOA->MODER |= GPIO_MODER_MODER5_0; // mode out GPIO 5
    GPIOA->OTYPER = 0; // output to Push/Pull,  no Pull-Up/Down
    GPIOA->OSPEEDR = 0; // low spoeed

    while (1) {
        // blink led
        GPIOA->ODR ^= GPIO_ODR_5;

        // delay current blink period
        for (volatile uint64_t i = 0; i < blink_curr; i++)
            ;
        uart_send("ABCDEFGH\r\n");
    }
}
