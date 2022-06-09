#pragma once
#include <stdint.h>

void uartQueue_init();
bool uartQueue_write(uint8_t element);
bool uartQueue_read(uint8_t* element);