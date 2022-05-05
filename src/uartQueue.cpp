
#include "uartQueue.h"
#include <stdint.h>

#define UART_QUEUE_SIZE 128

uint8_t uartQueue[UART_QUEUE_SIZE];
uint8_t* uartQueueRead;
uint8_t* uartQueueWrite;

//Init pointer to zero
void uartQueue_init()
{
    uartQueueRead = uartQueue;
    uartQueueWrite = uartQueue;
}

//Write to uartqueue
bool uartQueue_write(uint8_t element)
{
    if (uartQueueWrite == &uartQueue[UART_QUEUE_SIZE - 1]) {
        // Write pointer points to last element of queue
        if (uartQueueRead == uartQueue) {
            // Read pointer points to first element of queue --> Queue is full
            return false; //
        }
    } else {
        if (uartQueueWrite + 1 == uartQueueRead) {
            // Read pointer is write+1 --> Queue is full
            return false;
        }
    }
    // buffer is free, we can write
    *uartQueueWrite = element;

    if (uartQueueWrite == &uartQueue[UART_QUEUE_SIZE - 1]) {
        // we are at the end of the queue --> return to 0
        uartQueueWrite = uartQueue;
    } else {
        // we have room -->
        uartQueueWrite++;
    }
    return true;
}

//Read from uart
bool uartQueue_read(uint8_t* element)
{
    //if read = write --> fail
    if (uartQueueRead == uartQueueWrite) {
        //buffer is empty --> can't read
        return false;
    }
    *element = *uartQueueRead;
    //test for end of queue
    if (uartQueueRead == &uartQueue[UART_QUEUE_SIZE - 1]) {
        //jump to start of Queue
        uartQueueRead = uartQueue;
    } else {
        //we have space --> inc pointer
        uartQueueRead++;
    }
    return true;
}