#include "stm32f091xc.h"

void timer_init()
{

    // Timer 1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // RCC ON
    GPIOA->MODER |= GPIO_MODER_MODER8_1; // Alt.Function pin PA8
    GPIOA->AFR[1] |= 0x0002; // AF2 = TIM1_CH1

    TIM1->PSC = 16 - 1; // Prescaler, CK_INT/PSC+1=CK_CNT
    TIM1->ARR = 8 - 1; // Auto Reload Register = Limit/Start for Up/Down counter
    TIM1->CCR1 = 8 - 1; // Capture Compare Register 1

    TIM1->CCMR1 = TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1; // 011, toggle mode, toggle OC1REF when TIMx_CNT=TIMx_CCR
    TIM1->CCER = TIM_CCER_CC1E; // Polarity active hi, enable output OC1
    TIM1->BDTR = TIM_BDTR_MOE; // enable output (Main Output Enable)
    TIM1->CR1 |= TIM_CR1_CEN; // enable timer
}
