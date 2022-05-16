#include "can.h"
#include "clockConfig.h"
#include "commandHandler.h"
#include "dataOut.h"
#include "define.h"
#include "led.h"
#include "stm32f091xc.h"
#include "timer.h"
#include "uart.h"
#include "uartQueue.h"

canQueue canReceiveQueue;
canQueue canSendQueue;

int main(void)
{

    clockConfig();
    uartQueue_init();
    uart_init();
    can_init();
    can_baudrate(CAN_BAUD_250K);
    canQueue_init(&canReceiveQueue);
    canQueue_init(&canSendQueue);

    timer_init();
    led_init();
    led_green_off();

    while (1) {
        commandHandler();

        CanMsg canMsg;
        NVIC_DisableIRQ(CEC_CAN_IRQn);
        // write received messages via uart
        if (canQueue_read(&canReceiveQueue, &canMsg)) {
            NVIC_EnableIRQ(CEC_CAN_IRQn);
            dataOut(canMsg);
        }
        NVIC_EnableIRQ(CEC_CAN_IRQn);

        NVIC_DisableIRQ(CEC_CAN_IRQn);
        // write sent messages via can
        if (canQueue_read(&canSendQueue, &canMsg)) {
            NVIC_EnableIRQ(CEC_CAN_IRQn);
            can_send(canMsg);
        }
        NVIC_EnableIRQ(CEC_CAN_IRQn);
    }
}
