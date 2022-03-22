#include "can.h"
#include "stm32f091xc.h"
#include "uart.h"

void can_init()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_CANEN;

    GPIOB->MODER |= GPIO_MODER_MODER8_1;
    GPIOB->MODER |= GPIO_MODER_MODER9_1;

    GPIOB->AFR[1] |= 0x0004 << GPIO_AFRH_AFSEL8_Pos;
    GPIOB->AFR[1] |= 0x0004 << GPIO_AFRH_AFSEL9_Pos;

    // Leave Sleep mode and enter Initialization mode
    CAN->MCR |= CAN_MCR_INRQ;
    while (!(CAN->MSR & CAN_MSR_INAK))
        ;

    CAN->MCR &= ~CAN_MCR_SLEEP;
}

void can_baudrate(CanBaudrate baudrate)
{
    CAN->BTR = baudrate;
}

void can_open()
{
    // Enter Normal mode
    CAN->MCR &= ~CAN_MCR_INRQ;
    while (CAN->MSR & CAN_MSR_INAK)
        ;

    CAN->FMR |= CAN_FMR_FINIT;
    CAN->FA1R |= CAN_FA1R_FACT0;
    CAN->sFilterRegister[0].FR1 = 0x123 << 5 | 0xFF70U << 16;
    CAN->FMR &= ~CAN_FMR_FINIT;

    // FIFO message pending interrupt enable
    CAN->IER |= CAN_IER_FMPIE0;
    NVIC_EnableIRQ(CEC_CAN_IRQn);
}

void can_close()
{
    // Enter Initialization mode
    CAN->MCR |= CAN_MCR_INRQ;
    while (!(CAN->MSR & CAN_MSR_INAK))
        ;
}

void can_send(CanMsg canMsg)
{
    // Wait until Transmit mailbox 0 empty
    while (!(CAN->TSR & CAN_TSR_TME0))
        ;

    // DLC - Data Length Code (Limited to maximum 8 bytes)
    uint8_t dlc = canMsg.dlc > 8 ? 8 : canMsg.dlc;
    CAN->sTxMailBox[0].TDTR = dlc;

    if (canMsg.extended) {
        // Extended 29 bit CAN frame
        CAN->sTxMailBox[0].TIR = (canMsg.id << CAN_TI0R_EXID_Pos) | CAN_TI0R_IDE;
    } else {
        // Standard 11 bit CAN frame
        CAN->sTxMailBox[0].TIR = (canMsg.id << CAN_TI0R_STID_Pos);
    }
    if (canMsg.remote) {
        // Remote frame - no data is transmitted
        CAN->sTxMailBox[0].TIR |= CAN_TI0R_RTR | CAN_TI0R_TXRQ;
    } else {
        // Data frame
        CAN->sTxMailBox[0].TDLR = (canMsg.data[0] << CAN_TDL0R_DATA0_Pos);
        CAN->sTxMailBox[0].TDLR |= (canMsg.data[1] << CAN_TDL0R_DATA1_Pos);
        CAN->sTxMailBox[0].TDLR |= (canMsg.data[2] << CAN_TDL0R_DATA2_Pos);
        CAN->sTxMailBox[0].TDLR |= (canMsg.data[3] << CAN_TDL0R_DATA3_Pos);
        CAN->sTxMailBox[0].TDHR = (canMsg.data[4] << CAN_TDH0R_DATA4_Pos);
        CAN->sTxMailBox[0].TDHR |= (canMsg.data[5] << CAN_TDH0R_DATA5_Pos);
        CAN->sTxMailBox[0].TDHR |= (canMsg.data[6] << CAN_TDH0R_DATA6_Pos);
        CAN->sTxMailBox[0].TDHR |= (canMsg.data[7] << CAN_TDH0R_DATA7_Pos);

        CAN->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
    }
}

bool can_transmitMailboxEmpty()
{
    // true if Mailbox is Empty
    return (CAN->TSR & CAN_TSR_TME0);
}

extern "C" {
void CEC_CAN_IRQ_Handler(void)
{
    CanMsg canMsg;

    if ((CAN->RF0R & CAN_RF0R_FMP0) != 0) {
        // Received CAN frame successfully
        canMsg.extended = (CAN->sFIFOMailBox[0].RIR & CAN_RI0R_IDE_Msk) >> CAN_RI0R_IDE_Pos;
        canMsg.remote = (CAN->sFIFOMailBox[0].RIR & CAN_RI0R_RTR_Msk) >> CAN_RI0R_RTR_Pos;
        canMsg.dlc = (CAN->sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC_Msk) >> CAN_RDT0R_DLC_Pos;

        canMsg.data[0] = (CAN->sFIFOMailBox[0].RDLR & CAN_RDL0R_DATA0_Msk) >> CAN_RDL0R_DATA0_Pos;
        canMsg.data[1] = (CAN->sFIFOMailBox[0].RDLR & CAN_RDL0R_DATA1_Msk) >> CAN_RDL0R_DATA1_Pos;
        canMsg.data[2] = (CAN->sFIFOMailBox[0].RDLR & CAN_RDL0R_DATA2_Msk) >> CAN_RDL0R_DATA2_Pos;
        canMsg.data[3] = (CAN->sFIFOMailBox[0].RDLR & CAN_RDL0R_DATA3_Msk) >> CAN_RDL0R_DATA3_Pos;
        canMsg.data[4] = (CAN->sFIFOMailBox[0].RDHR & CAN_RDH0R_DATA4_Msk) >> CAN_RDH0R_DATA4_Pos;
        canMsg.data[5] = (CAN->sFIFOMailBox[0].RDHR & CAN_RDH0R_DATA5_Msk) >> CAN_RDH0R_DATA5_Pos;
        canMsg.data[6] = (CAN->sFIFOMailBox[0].RDHR & CAN_RDH0R_DATA6_Msk) >> CAN_RDH0R_DATA6_Pos;
        canMsg.data[7] = (CAN->sFIFOMailBox[0].RDHR & CAN_RDH0R_DATA7_Msk) >> CAN_RDH0R_DATA7_Pos;

        // Release Fifo
        CAN->RF0R |= CAN_RF0R_RFOM0;

        uart_send("Received\r\n");
    }
}
}