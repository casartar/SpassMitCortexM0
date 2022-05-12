#include "can.h"
#include "clockConfig.h"
#include "commandHandler.h"
#include "led.h"
#include "stm32f091xc.h"
#include "timer.h"
#include "uart.h"
#include "uartQueue.h"

int main(void)
{

    clockConfig();
    uartQueue_init();
    uart_init();
    can_init();
    can_baudrate(CAN_BAUD_250K);

    timer_init();
    led_init();
    led_green_off();

    while (1) {
        commandHandler();
    }
}
