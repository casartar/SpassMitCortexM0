#include "led.h"
#include "stm32f091xc.h"

void led_init()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOA->MODER |= GPIO_MODER_MODER15_0; //Def LED_Green
    GPIOB->MODER |= GPIO_MODER_MODER5_0; //Def LED_Rot
    led_green_off();
    led_red_off();
}

void led_red_off()
{
    GPIOA->ODR |= GPIO_ODR_15;
}

void led_red_on()
{
    GPIOA->ODR &= ~GPIO_ODR_15;
}
void led_green_off()
{
    GPIOB->ODR |= GPIO_ODR_5;
}

void led_green_on()
{
    GPIOB->ODR &= ~GPIO_ODR_5;
}

void led_red_toggle()
{
    GPIOA->ODR ^= GPIO_ODR_15;
}
void led_green_toggle()
{
    GPIOB->ODR ^= GPIO_ODR_5;
}