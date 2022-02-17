#include "stm32f091xc.h"

int main (void) {
   

    RCC->AHBENR     |= RCC_AHBENR_GPIOAEN; //RCC ON

    GPIOA->MODER    |= GPIO_MODER_MODER5_0; //mode out
    GPIOA->OTYPER   = 0;
    GPIOA->OSPEEDR  = 0;
    
    // UART Clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // UART GPIO
    GPIOA->MODER |= GPIO_MODER_MODER9_1; // Pin 9 Alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODER10_1; // Pin 10 Alternate function mode

    GPIOA->AFR[1] |= 0x0001 << GPIO_AFRH_AFSEL9_Pos; // Pin 9 AF1
    GPIOA->AFR[1] |= 0x0001 << GPIO_AFRH_AFSEL10_Pos; // Pin 10 AF1
    // UART Config
    USART1->BRR = SystemCoreClock/9600; // Baudrate
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE; // Enable 

    while (1) 
    {
        GPIOA->ODR ^= GPIO_ODR_5;
        for (volatile uint64_t i = 0; i < 100000; i++);

        // UART Transmit
        USART1->TDR = 0x55;
    }
}