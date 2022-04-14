#include "can.h"
#include "clockConfig.h"
#include "stm32f091xc.h"
#include "timer.h"
#include "uart.h"

int main(void)
{
    clockConfig();
    uart_init();
    can_init();
    can_baudrate(CAN_BAUD_250K);
    can_open();

    timer_init();

    while (1) {
    }
}
