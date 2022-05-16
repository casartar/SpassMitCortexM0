
#include "canQueue.h"
#include "can.h"

// Init pointer to zero
void canQueue_init(canQueue* q)
{
    q->canQueueRead = q->canQueue;
    q->canQueueWrite = q->canQueue;
}

// Write to Can Queue
bool canQueue_write(canQueue* q, CanMsg element)
{
    if (q->canQueueWrite == &q->canQueue[CAN_QUEUE_SIZE - 1]) {
        // Write pointer points to last element of queue
        if (q->canQueueRead == q->canQueue) {
            // Read pointer points to first element of queue --> Queue is full
            return false; //
        }
    } else {
        if (q->canQueueWrite + 1 == q->canQueueRead) {
            // Read pointer is write+1 --> Queue is full
            return false;
        }
    }
    // buffer is free, we can write
    q->canQueueWrite->id = element.id;
    q->canQueueWrite->remote = element.remote;
    q->canQueueWrite->dlc = element.dlc;
    q->canQueueWrite->extended = element.extended;
    for (uint8_t i = 0; i < 8; i++) {
        q->canQueueWrite->data[i] = element.data[i];
    }

    if (q->canQueueWrite == &q->canQueue[CAN_QUEUE_SIZE - 1]) {
        // we are at the end of the queue --> return to 0
        q->canQueueWrite = q->canQueue;
    } else {
        // we have room -->
        q->canQueueWrite++;
    }
    return true;
}

// Read from can
bool canQueue_read(canQueue* q, CanMsg* element)
{
    // if read = write --> fail
    if (q->canQueueRead == q->canQueueWrite) {
        // buffer is empty --> can't read
        return false;
    }
    element->id = q->canQueueRead->id;
    element->remote = q->canQueueRead->remote;
    element->dlc = q->canQueueRead->dlc;
    element->extended = q->canQueueRead->extended;
    for (uint8_t i = 0; i < 8; i++) {
        element->data[i] = q->canQueueRead->data[i];
    }

    // test for end of queue
    if (q->canQueueRead == &q->canQueue[CAN_QUEUE_SIZE - 1]) {
        // jump to start of Queue
        q->canQueueRead = q->canQueue;
    } else {
        // we have space --> inc pointer
        q->canQueueRead++;
    }
    return true;
}