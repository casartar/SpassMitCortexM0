#include "stm32f091xc.h"

void can_init()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_CANEN;

    GPIOB->MODER |= GPIO_MODER_MODER8_1;
    GPIOB->MODER |= GPIO_MODER_MODER9_1;

    GPIOB->AFR[1] |= 0x0004 << GPIO_AFRH_AFSEL8_Pos;
    GPIOB->AFR[1] |= 0x0004 << GPIO_AFRH_AFSEL9_Pos;

    // Enter Initialization mode
    CAN->MCR |= CAN_MCR_INRQ;
    while (!(CAN->MSR & CAN_MSR_INAK))
        ;
    //
    CAN->MCR &= ~CAN_MCR_SLEEP;

    CAN->BTR = 0x05 | (0x0C << CAN_BTR_TS1_Pos) | (0x01 << CAN_BTR_TS1_Pos);

    // Enter Normal mode
    CAN->MCR &= ~CAN_MCR_INRQ;
    while (CAN->MSR & CAN_MSR_INAK)
        ;

    while (1) {
        if (CAN->TSR & CAN_TSR_TME0) {
            // Transmit mailbox 0 empty
            CAN->sTxMailBox[0].TDTR = 1; // Set DLC to 1
            CAN->sTxMailBox[0].TDLR = 0x01; // Write 0x01 in the first byte
            CAN->sTxMailBox[0].TIR = (uint32_t)(0x01 << 21 | CAN_TI0R_TXRQ); // Set CAN ID and Transmit mailbox request
        }
    }
}