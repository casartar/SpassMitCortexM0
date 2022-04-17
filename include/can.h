#pragma once

#include <stdint.h>

struct CanMsg {
    uint32_t id = 0;
    uint8_t dlc = 0;
    bool extended = false;
    bool remote = false;
    uint8_t data[8] = {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
};

enum CanBaudrate {
    // calculated with http://www.bittiming.can-wiki.info/

    CAN_BAUD_10K = 0x001c012b, // Prescaler: 300, Timequanta: 16, Seg1: 13, Seg2: 2 SamplePoint: 87.5
    CAN_BAUD_20K = 0x001c0095, // Prescaler: 150, Timequanta: 16, Seg1: 13, Seg2: 2 SamplePoint: 87.5
    CAN_BAUD_50K = 0x001c003b, // Prescaler: 60, Timequanta: 16, Seg1: 13, Seg2: 2 SamplePoint: 87.5
    CAN_BAUD_100K = 0x001c001d, // Prescaler: 30, Timequanta: 16, Seg1: 13, Seg2: 2 SamplePoint: 87.5
    CAN_BAUD_125K = 0x001c0017, // Prescaler: 24, Timequanta: 16, Seg1: 13, Seg2: 2 SamplePoint: 87.5
    CAN_BAUD_250K = 0x001c000b, // Prescaler: 12, Timequanta: 16, Seg1: 13, Seg2: 2 SamplePoint: 87.5
    CAN_BAUD_500K = 0x001c0005, // Prescaler: 6, Timequanta: 16, Seg1: 13, Seg2: 2 SamplePoint: 87.5
    CAN_BAUD_800K = 0x001b0003, // Prescaler: 4, Timequanta: 15, Seg1: 12, Seg2: 2 SamplePoint: 86.7
    CAN_BAUD_1M = 0x001c0002, // Prescaler: 3, Timequanta: 16, Seg1: 13, Seg2: 2 SamplePoint: 87.5
};

void can_init();
void can_baudrate(CanBaudrate baudrate);
void can_open();
void can_close();
void can_send(CanMsg canMsg);
bool can_transmitMailboxEmpty();
