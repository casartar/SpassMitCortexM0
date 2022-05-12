#include "commandHandler.h"
#include "can.h"
#include "define.h"
#include "led.h"
#include "stm32f091xc.h"
#include "uart.h"
#include "uartQueue.h"
#include <stdlib.h>
#include <string.h>

bool allCharsInStringAreZero(char* string);

bool canStatusOpened = false;
bool canBaudrateAlreadySet = false;
char cmdString[CMD_MAX_LEN];

void commandHandler()
{
    static uint8_t cmdStringIndex = 0;
    bool error = false;
    uint8_t character;

    NVIC_DisableIRQ(USART1_IRQn);
    if (!uartQueue_read(&character)) {
        //Queue is empty, do nothing
        return;
    }
    NVIC_EnableIRQ(USART1_IRQn);

    // Write character to Buffer, if length smaller than or equal to maximum buffer size (CMD_MAX_LEN)
    if (cmdStringIndex >= CMD_MAX_LEN) {
        if (character != '\r') {
            return;
        } else {
            cmdString[CMD_MAX_LEN - 1] = '\0';
        }
    } else {
        if (character == '\r') {
            //If carriage return, replace with NUL
            cmdString[cmdStringIndex] = '\0';
            cmdStringIndex = 0;
        } else {
            cmdString[cmdStringIndex++] = character;
            return;
        }
    }

    switch (cmdString[0]) {
    // S - Command
    case 'S':
        switch (cmdString[1]) {
        case '0':
            if (cmdString[2] == '\0' && !canStatusOpened) {
                can_baudrate(CAN_BAUD_10K);
                canBaudrateAlreadySet = true;
            } else {
                error = true;
            }
            break;
        case '1':
            if (cmdString[2] == '\0' && !canStatusOpened) {
                can_baudrate(CAN_BAUD_20K);
                canBaudrateAlreadySet = true;
            } else {
                error = true;
            }
            break;
        case '2':
            if (cmdString[2] == '\0' && !canStatusOpened) {
                can_baudrate(CAN_BAUD_50K);
                canBaudrateAlreadySet = true;
            } else {
                error = true;
            }
            break;
        case '3':
            if (cmdString[2] == '\0' && !canStatusOpened) {
                can_baudrate(CAN_BAUD_100K);
                canBaudrateAlreadySet = true;
            } else {
                error = true;
            }
            break;
        case '4':
            if (cmdString[2] == '\0' && !canStatusOpened) {
                can_baudrate(CAN_BAUD_125K);
                canBaudrateAlreadySet = true;
            } else {
                error = true;
            }
            break;
        case '5':
            if (cmdString[2] == '\0' && !canStatusOpened) {
                can_baudrate(CAN_BAUD_250K);
                canBaudrateAlreadySet = true;
            } else {
                error = true;
            }
            break;
        case '6':
            if (cmdString[2] == '\0' && !canStatusOpened) {
                can_baudrate(CAN_BAUD_500K);
                canBaudrateAlreadySet = true;
            } else {
                error = true;
            }
            break;
        case '7':
            if (cmdString[2] == '\0' && !canStatusOpened) {
                can_baudrate(CAN_BAUD_800K);
                canBaudrateAlreadySet = true;
            } else {
                error = true;
            }
            break;
        case '8':
            if (cmdString[2] == '\0' && !canStatusOpened) {
                can_baudrate(CAN_BAUD_1M);
                canBaudrateAlreadySet = true;
            } else {
                error = true;
            }
            break;
        default:
            error = true;
            break;
        }
        break;
        // S - Open
    case 'O':
        if (cmdString[1] == '\0' && !canStatusOpened && canBaudrateAlreadySet) {
            can_open();
            canStatusOpened = true;
            led_green_on();
        } else {
            error = true;
        }
        break;
        // C - Close
    case 'C':
        if (cmdString[1] == '\0' && canStatusOpened) {
            can_close();
            canStatusOpened = false;
            led_green_off();
        } else {
            error = true;
        }
        break;
    case 't':
    case 'T':
        if (canStatusOpened) {
            uint8_t idLen = cmdString[0] == 't' ? 3 : 8;
            uint8_t tempString[9];
            uint8_t dlc;
            tempString[1] = '\0';
            CanMsg canMsg;
            canMsg.extended = cmdString[0] == 't' ? false : true;

            tempString[0] = cmdString[idLen + 1];
            dlc = atoi((char*)tempString);
            if (dlc <= 8 && cmdString[idLen + 2 + dlc * 2] == '\0' && !(dlc == 0 && tempString[0] != '0')) {
                strncpy((char*)tempString, (char*)&cmdString[1], idLen);
                tempString[idLen] = '\0';

                char* ptr;
                uint32_t identifier = strtol((char*)tempString, &ptr, 16);
                if (!(identifier == 0 && !allCharsInStringAreZero((char*)tempString))) {
                    if ((identifier < 0x1FFFFFFF && cmdString[0] == 'T') || (identifier < 0x7FF && cmdString[0] == 't')) {
                        canMsg.id = identifier;
                        canMsg.dlc = dlc;
                        canMsg.remote = false;
                        for (uint8_t i = 0; i < dlc; i++) {
                            strncpy((char*)tempString, (char*)&cmdString[idLen + 2 + i * 2], 2);
                            tempString[2] = '\0';
                            canMsg.data[i] = strtol((char*)tempString, &ptr, 16);
                            if (canMsg.data[i] == 0 && !allCharsInStringAreZero((char*)tempString)) {
                                error = true;
                            }
                        }
                        if (!error) {
                            can_send(canMsg);
                        }

                    } else {
                        error = true;
                    }
                } else {
                    error = true;
                }

            } else {
                error = true;
            }
        } else {
            error = true;
        }
        break;

    default:
        error = true;
        break;
    }
    if (error) {
        uart_send("\a");
    } else {
        uart_send("\r");
    }
}

bool allCharsInStringAreZero(char* string)
{
    while (*string != '\0') {
        if (*string++ != '0') {
            return false;
        }
    }
    return true;
}