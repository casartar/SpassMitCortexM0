#include "stm32f091xc.h"

int main (void) {
   

    RCC->AHBENR     |= RCC_AHBENR_GPIOAEN; //RCC ON

    GPIOA->MODER    |= GPIO_MODER_MODER5_0; //mode out
    GPIOA->OTYPER   = 0;
    GPIOA->OSPEEDR  = 0;
    
    while (1) 
    {

        GPIOA->ODR ^= GPIO_ODR_5;

        for (volatile uint64_t i = 0; i < 1000000000000; i++);
        
 
    }
}