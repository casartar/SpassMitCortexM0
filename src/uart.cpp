#include "stm32f091xc.h"
#include "string.h"
#define TX_BUFFERSIZE 256

volatile char uart_txBuffer[TX_BUFFERSIZE];
volatile uint16_t txIndex;
volatile uint16_t strLength;

void uart_init()
{
    //UART CLock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //RCC ON
    // UART GPIO
    GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1; // Pin 9 und 10 Alternate function mode
    GPIOA->AFR[1] |= 0x0001 << GPIO_AFRH_AFSEL9_Pos; // Pin 9 AF1
    GPIOA->AFR[1] |= 0x0001 << GPIO_AFRH_AFSEL10_Pos; //Pin 10 AF1
    //UART Config
    USART1->BRR = SystemCoreClock / 9600;
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE /*| USART_CR1_RE | USART_CR1_RXNEIE*/;
    NVIC_EnableIRQ(USART1_IRQn);
}

void uart_send(char* string)
{
    while (!(USART1->ISR & USART_ISR_TC))
        ;
    //Wait while Transmission not complete
    strcpy((char*)uart_txBuffer, string); //TODO: Handling Bufferoverflow
    //set transmission Flag
    strLength = strlen(string); // Set index length
    txIndex = 1; // Reset Index to 1
    USART1->TDR = uart_txBuffer[0]; //Transmit first byte, transmit interrupt will be triggered.
    USART1->CR1 |= USART_CR1_TXEIE;
}

extern "C" {
void USART1_IRQHandler(void)
{ // Interrupt for lower to upper case

    // did we receive a char?
    if (USART1->ISR & USART_ISR_RXNE) {
        volatile uint8_t rxchar = (uint8_t)(USART1->RDR); //read char, clear interrupt flag
    }
    // did we set the send flag?
    if (USART1->ISR & USART_ISR_TXE) {
        //define Buffer length
        if (txIndex < strLength) {
            USART1->TDR = uart_txBuffer[txIndex++];
        } else {
            USART1->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
}