#include "stm32f091xc.h"
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
    uart_init();
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //RCC ON

    GPIOA->MODER |= GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0; // mode out GPIO 5 and 6

    GPIOA->OTYPER = 0;
    GPIOA->OSPEEDR = 0;

    GPIOA->ODR ^= GPIO_ODR_6;
    // Timer 1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    // RCC->GPIOA is already enabled
    GPIOA->MODER |= GPIO_MODER_MODER8_1; // Alt.Function pin PA8
    GPIOA->AFR[1] |= 0x0002; // AF2 = TIM1_CH1
    TIM1->PSC = 16 - 1; // Prescaler, CK_INT/PSC+1=CK_CNT
    TIM1->ARR = 8 - 1; // Auto Reload Register = Limit/Start for Up/Down counter
    TIM1->CCR1 = 8 - 1; // Capture Compare Register 1
    /*
    16, 2, 2 = 125 kHz
     8, 2, 2 = 250 kHz
     4, 2, 2 = 500 kHz usw.

    16, 2, 4 = invalid
    16, 4, 4 = 62.5 kHz
    16, 1, 1 = invalid
    16, 8, 4 = 31.25 kHz
    */
    TIM1->CCMR1 = TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1; // 011, toggle mode, toggle OC1REF when TIMx_CNT=TIMx_CCR
    TIM1->CCER = TIM_CCER_CC1E; // Polarity active hi, enable output OC1
    TIM1->BDTR = TIM_BDTR_MOE; // enable output (Main Output Enable)
    TIM1->CR1 |= TIM_CR1_CEN; // enable timer

    while (1) {
        // blink led
        GPIOA->ODR ^= GPIO_ODR_5;

        // delay current blink period
        for (volatile uint64_t i = 0; i < blink_curr; i++)
            ;
        uart_send("ABCDEFGH\r\n");
    }
}
