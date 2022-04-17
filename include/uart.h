#pragma once

#define TX_BUFFERSIZE 256
#define RX_BUFFERSIZE 256

extern volatile bool uart_rxReceiveComplete;
extern volatile char uart_rxBuffer[RX_BUFFERSIZE];

void uart_init();
void uart_send(char* string);
