#pragma once
#include "can.h"
#include <stdint.h>
#define CAN_QUEUE_SIZE 128

struct canQueue {
    // Placeholder
    volatile CanMsg canQueue[CAN_QUEUE_SIZE];
    volatile CanMsg* canQueueRead;
    volatile CanMsg* canQueueWrite;
};

void canQueue_init(canQueue* q);
bool canQueue_write(canQueue* q, CanMsg element);
bool canQueue_read(canQueue* q, CanMsg* element);
