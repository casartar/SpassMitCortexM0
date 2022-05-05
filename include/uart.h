#pragma once

#define TX_BUFFERSIZE 256

void uart_init();
void uart_send(char* string);
