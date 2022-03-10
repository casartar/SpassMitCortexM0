#include "stm32f091xc.h"

void can_init()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_CANEN;

    GPIOB->MODER |= GPIO_MODER_MODER8_1;
    GPIOB->MODER |= GPIO_MODER_MODER9_1;

    GPIOB->AFR[1] |= 0x0004 << GPIO_AFRH_AFSEL8_Pos;
    GPIOB->AFR[1] |= 0x0004 << GPIO_AFRH_AFSEL9_Pos;

    CAN->MCR |= CAN_MCR_INRQ;
    while (!(CAN->MSR & CAN_MSR_INAK))
        ;
    CAN->MCR &= ~CAN_MCR_SLEEP;

    CAN->BTR = 0x05 | (0x0C << CAN_BTR_TS1_Pos) | (0x01 << CAN_BTR_TS1_Pos);
}