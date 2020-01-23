;/* 
 * File:   backgroundTasks.c
 * Author: Bruno Cornil
 *
 * Created on 25 décembre 2015, 22:34
 */

#include <stdio.h>
#include <stdlib.h>

#include "../PrcLib/PRC.h"		//  PRC.h contains the prototype of PrcInit() and timing functions
#include "../PrcLib/AMB2300.h"	//  Radio.h contains the prototypes of Bluetooth radio functions
#include "../PrcLib/LEDs.h"		//  LEDs.h contains the prototypes of LEDs functions

#include "robot.h"
#include "interpreter.h"        //  commands.h contains the prototypes of command interpreter function
#include "protocol.h"
#include "realTimeTasks.h"      //  realTimeTasks.h contains the prototypes of realtime tasks functions/structures
#include "backgroundTasks.h"

int isSerial=0;
long event_timer=0;

/*
 * Process log data from RT task
 */
void processRealTimeLogs(void) {
    struct sLogQueue logData;
    char data[5];
    int i;
    
    while (realTime_LogPull(&logData) != 0) {
        data[0]=logData.id;
        for (i=1;i<5;i++){
            data[i]=(logData.value & 0xff);
            logData.value = logData.value >> 8;
        }
        sendCommand(CMD_RT_LOG_R, 5,data);
    }
}

/*
 * Gather status from various parts of the code
 */
int getStatus(void) {
    int status;
    
    status=0;
    if (timerRead() > event_timer)
        status |= EVENTS_TIMER;
    if (!realTime_isMoving())
        status |= EVENTS_MOTOR_OFF; 
    if (!realTime_isOdometerResetRequest())
        status |= EVENTS_ODO_RESET;
    if (!realTime_isTurntableMoving())
        status |= EVENTS_TURN_OFF;
    return status;
}

/*
 * Initialize background task
 */
void background_Init(void) {      
    isSerial=0;
    
    timerStart();
} 

/*
 * Set background timer
 */
void background_SetTimer(int sleepTime) {
    event_timer = timerRead()+sleepTime;
}

/*
 * Manage background activities (Bluetooth command ifce, logs, ...)
 */
void background_WaitFor(int conditions) {
    int isWait=1;
    
    // Loop at least once and until all conditions are met
    while (isWait) {
        if (isSerial == 0) {
            // Wait for a radio connection to process commands
            if (radioGetStatus() == RADIO_CONNECTED){
                isSerial = 1;
                initCommandInterface();
                ledWrite(LED_RADIO, 1);
            } else {
                ledToggle(LED_RADIO);
                delay_ms(50);
            }
        } else {
            if (radioGetStatus() != RADIO_CONNECTED){
                isSerial = 0;
            } else {
                // Process new commands
                processCommand();
                // Handle logs from RT tasks
                processRealTimeLogs();           
            }
        }
        // Check if all conditions are met
        isWait = 1;
        if ((getStatus() & conditions) == conditions)
            isWait = 0;
    }
}

