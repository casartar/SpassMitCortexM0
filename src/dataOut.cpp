#include "dataOut.h"
#include "stdio.h"
#include "uart.h"

void dataOut(CanMsg canMsg)
{
    char canMsgString[sizeof("T1111222281122334455667788EA5F\r") + 1];

    // 't' on standard identifier - 'T' on extended identifier
    if (canMsg.extended) {
        sprintf(canMsgString, "T%08X%X", canMsg.id, canMsg.dlc);
        for (uint8_t i = 0; i < canMsg.dlc; i++) {
            sprintf(&canMsgString[10 + i * 2], "%02X", canMsg.data[i]);
        }
        sprintf(&canMsgString[10 + canMsg.dlc * 2], "\r");
    } else {
        sprintf(canMsgString, "t%03X%X", canMsg.id, canMsg.dlc);
        for (uint8_t i = 0; i < canMsg.dlc; i++) {
            sprintf(&canMsgString[5 + i * 2], "%02X", canMsg.data[i]);
        }
        sprintf(&canMsgString[5 + canMsg.dlc * 2], "\r");
    }
    uart_send(canMsgString);
}