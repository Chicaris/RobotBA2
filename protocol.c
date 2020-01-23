/* 
 * File:   command.c
 * Author: Bruno Cornil
 *
 * Created on 1 décembre 2015, 15:27
 */

#include <stdio.h>
#include <stdlib.h>

#include "../PrcLib/AMB2300.h"	//  Radio.h contains the prototypes of Bluetooth radio functions

#define CMD_EOM 0xa5
#define CMD_ESC 0x1b
#define ESC_EOM 0x01
#define ESC_ESC 0x02

#define CMD_BUFFERSIZE 32

// Internal static variables
unsigned char cmdBuffer[CMD_BUFFERSIZE]={0};
unsigned int  cmdWriteIdx=0;
unsigned int  isEscape=0;

/*
 *  Initialize protocol buffer
 */
void initProtocolBuffer(void) {
    cmdWriteIdx=0;
    isEscape=0;
}

/*
 * Read a command string from serial interface (string terminate with byte CMD_EOM)
 */
unsigned char* getCommand(void) {

    unsigned char   rxByte;
    unsigned char* rv;
    
    rv=NULL;
    if (radioGetRxBufferSpace() > 0) {
        rxByte = radioGetChar();
        switch (rxByte) {
            // End of Message
            case CMD_EOM:
                rv=cmdBuffer;
                cmdWriteIdx=0;      
                break;
            // Escape character, wait for the next one
            case CMD_ESC:
                isEscape=1;
                break;
            default:
                // Substitute Escaped character
                if (isEscape==1) {
                    isEscape=0;
                    switch (rxByte) {
                        case ESC_EOM:
                            rxByte=CMD_EOM;
                            break;
                        case ESC_ESC:
                            rxByte=CMD_ESC;
                            break;
                    }
                }
                cmdBuffer[cmdWriteIdx++]=rxByte;
                break;
        }
        // Circular buffer to avoid overflow
        if (cmdWriteIdx == CMD_BUFFERSIZE) {
            cmdWriteIdx=0;
        }
    }
    return rv;
}

/*
 * Send character or escape it if ESC or EOM
 */
void sendEncodedChar(unsigned char data) {
    switch (data) {
        // EOM has been seen, encode with ESC+ESC(EOM))
        case CMD_EOM:
            radioSendChar(CMD_ESC);
            radioSendChar(ESC_EOM);
            break;
        // ESC has been seen, encode with ESC+ESC(ESC))
        case CMD_ESC:
            radioSendChar(CMD_ESC);
            radioSendChar(ESC_ESC);
            break;
        // Not a special character, send untouched
        default:
            radioSendChar(data);
    }    
}

void sendCommand(char command, int size, char *data) {
    int i;
    
    // Command is COMMAND_ID + DATA + EOM
    sendEncodedChar(command);
    for (i=0; i<size; i++) {
        sendEncodedChar(data[i]);
    }
    radioSendChar(CMD_EOM);
}



