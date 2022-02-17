#include "stm32f091xc.h"

int main (void) {
   

    RCC->AHBENR     |= RCC_AHBENR_GPIOAEN; //RCC ON

    GPIOA->MODER    |= GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0; //mode out GPIO 5 and 6

    GPIOA->OTYPER   = 0;
    GPIOA->OSPEEDR  = 0;
    
    //UART CLock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // UART GPIO
    GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1; // Pin 9 und 10 Alternate function mode
    GPIOA->AFR[1] |= 0x0001 << GPIO_AFRH_AFSEL9_Pos; // Pin 9 AF1
    GPIOA->AFR[1] |= 0x0001 << GPIO_AFRH_AFSEL10_Pos; //Pin 10 AF1
    //UART Config
    USART1->BRR = SystemCoreClock/9600;
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE | USART_CR1_RE;
    char charToReceive = 0x55;

    GPIOA->ODR ^= GPIO_ODR_6;
    while (1) 
    {

        GPIOA->ODR ^= GPIO_ODR_5 | GPIO_ODR_6;
        for (volatile uint64_t i = 0; i < 100000; i++);
        //UART Receive
        if ((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
        {
            charToReceive = (uint8_t)(USART1->RDR);
        }
        //UART transmit
        USART1->TDR = charToReceive;
    }
}