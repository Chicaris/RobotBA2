/* 
 * File:   mission.c
 * Author: Bruno Cornil
 *
 * Created on 25 décembre 2015, 22:34
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../PrcLib/motors.h"
#include "backgroundTasks.h"
#include "realTimeTasks.h"
#include "protocol.h"
#include "interpreter.h"
#include "robot.h"

#define MISSION_DONE  0
#define MISSION_START 1
#define MISSION_FAIL  2

#define BASKET -1
#define TRASH 1

#define Y_DISTRIBUTION 40
#define X_DISTRIBUTION -180

#define LENGHTBARCODE 22
#define Y_BARCODE 70

#define NEGGS 10
#define NPOSITIONS 5

int isStartMission=0;

struct sPositionList {
    int x;
    int y;
};

/*******************************************************************************
 * Distribution processing
 ******************************************************************************/

void distribution (int mode){
    
    realtime_turntable_next();
    background_WaitFor(EVENTS_TURN_OFF);
    
    motorSetVoltage(MOTOR_BAND,mode*MOTOR_BAND_SPEED);
    motorEnable(MOTOR_BAND);

    // Wait for 1s
    background_SetTimer(3000);
    background_WaitFor(EVENTS_TIMER);
    
    motorSetVoltage(MOTOR_BAND,0);
    motorDisable(MOTOR_BAND);
}
/*
void strat_mission(void){
    char mission_status;
    int barCode; 
    
    mission_status = MISSION_START;
    barCode = -1;
    sendCommand(CMD_GO_R, 1,&mission_status);
    
    // Reset Odometer (and wait for)
    realTime_odometerReset();
    background_WaitFor(EVENTS_ODO_RESET);

    // Enable turntable regulation
    realtime_turntable_enable();
    realTime_barcode_arm(BARCODE_TYPE_SINGLE);
    realTime_goto(0,30);
    background_WaitFor(EVENTS_MOTOR_OFF);
    barCode = realTime_barcode_read();
    sendCommand(CMD_CBARRE_READ_R, 2, (char *)&barCode);
    realtime_turntable_disable();   
}

*/
 /*
 * Start main mission
 */

void start_mission(void) {
    char mission_status;
    int i;
    int barCode; 
    int listBarcode[5];

    struct sPositionList barCodeStartPosition[5] = {
        {-18,Y_BARCODE},
        {-41,Y_BARCODE},
        {-64,Y_BARCODE-1},
        {-87,Y_BARCODE-2},
        {-111,Y_BARCODE-3}
    };
    struct sPositionList listCoordDistri[5] = {
        {-40,Y_DISTRIBUTION-15},
        {-65,Y_DISTRIBUTION-13},
        {-90,Y_DISTRIBUTION-11},
        {-115,Y_DISTRIBUTION-9},
        {-140,Y_DISTRIBUTION-7}
    };
    int listEggs[] = {1,1,2,2,3,3,4,4,5,5};

    // Send Mission start message
    mission_status = MISSION_START;
    sendCommand(CMD_GO_R, 1,&mission_status);
    
    // Reset Odometer (and wait for)
    realTime_odometerReset();
    background_WaitFor(EVENTS_ODO_RESET);

    // Enable turntable regulation
    realtime_turntable_enable();
    
    //Scan of the double Barcode
    barCode = -1;

    while(barCode==-1) {
        realTime_goto(0,30);
        background_WaitFor(EVENTS_MOTOR_OFF);
        realTime_barcode_arm(BARCODE_TYPE_DOUBLE);
        realTime_goto(0,60);
        background_WaitFor(EVENTS_MOTOR_OFF);
        barCode = realTime_barcode_read();
        sendCommand(CMD_CBARRE_READ_R, 2, (char *)&barCode);
        if (barCode != -1)
            realTime_goto(-10,75);
        background_WaitFor(EVENTS_MOTOR_OFF);
    }
    
    
    //Scan single barcode
    mission_status = MISSION_DONE;
    for(i=0;i<5;i++){
        listBarcode[i]= -1;
        if (barCode & (1<<i)){
            while(listBarcode[i]==-1) {
                realTime_goto(barCodeStartPosition[i].x,barCodeStartPosition[i].y);
                background_WaitFor(EVENTS_MOTOR_OFF);
                realTime_barcode_arm(BARCODE_TYPE_SINGLE);
                realTime_goto(barCodeStartPosition[i].x-LENGHTBARCODE,barCodeStartPosition[i].y);
                background_WaitFor(EVENTS_MOTOR_OFF);
                listBarcode[i]= realTime_barcode_read();
                if (listBarcode[i]>5)
                    listBarcode[i] = -1;
                if(listBarcode[i]==-1)
                    mission_status=MISSION_FAIL;
                sendCommand(CMD_CBARRE_READ_R, 2, (char *)&listBarcode[i]);
            }
        }
    }
    
    
    //Distribution y position
    realTime_goto(-170,Y_BARCODE);
    background_WaitFor(EVENTS_MOTOR_OFF);        
    realTime_goto(-170,Y_DISTRIBUTION);
    background_WaitFor(EVENTS_MOTOR_OFF);
    
    int egg;
    int position;
    for(egg=0;egg<NEGGS;egg++) {
        position=0;
        while (position<NPOSITIONS && listBarcode[position]!=listEggs[egg])
            position++;
        if (position < NPOSITIONS) {
            // Distribute
            listBarcode[position]=-1;
            realTime_goto(listCoordDistri[position].x,listCoordDistri[position].y);
            background_WaitFor(EVENTS_MOTOR_OFF);
            distribution(BASKET);
       } else {
            // Trash
            distribution(TRASH);
        }
    }
    // Disable turntable regulation
    realtime_turntable_disable();
    
    //move to (0,0)
    realTime_goto(0,Y_DISTRIBUTION-10);
    background_WaitFor(EVENTS_MOTOR_OFF);
    realTime_goto(0,0);
    background_WaitFor(EVENTS_MOTOR_OFF);
    
    // Send Mission complete/failed message
    sendCommand(CMD_GO_R, 1,&mission_status);
}
    


/*
 void distribution (int mode){
    
    realtime_turntable_next();
    background_WaitFor(EVENTS_TURN_OFF);
    
    motorSetVoltage(MOTOR_BAND,mode*MOTOR_BAND_SPEED);
    motorEnable(MOTOR_BAND);

    // Wait for 1s
    background_SetTimer(3000);
    background_WaitFor(EVENTS_TIMER);
    
    motorSetVoltage(MOTOR_BAND,0);
    motorDisable(MOTOR_BAND);
}


 * Start main mission
 
void start_mission(void) {
    char mission_status;
    int i;
    int barCode; 
    int listBarcode[5];

    struct sPositionList barCodeStartPosition[5] = {
        {-18,Y_BARCODE},
        {-41,Y_BARCODE},
        {-67,Y_BARCODE},
        {-89,Y_BARCODE},
        {-111,Y_BARCODE}
    };
    struct sPositionList listCoordDistri[5] = {
        {-40,Y_DISTRIBUTION+5},
        {-65,Y_DISTRIBUTION+4},
        {-90,Y_DISTRIBUTION+3},
        {-115,Y_DISTRIBUTION+2},
        {-140,Y_DISTRIBUTION+1}
    };
    int listEggs[] = {1,1,2,2,3,3,4,4,5,5};

    // Send Mission start message
    mission_status = MISSION_START;
    sendCommand(CMD_GO_R, 1,&mission_status);
    
    // Reset Odometer (and wait for)
    realTime_odometerReset();
    background_WaitFor(EVENTS_ODO_RESET);

    // Enable turntable regulation
    realtime_turntable_enable();
    
    //Scan of the double Barcode
    barCode = -1;

    while(barCode==-1) {
        realTime_goto(0,30);
        background_WaitFor(EVENTS_MOTOR_OFF);
        realTime_barcode_arm(BARCODE_TYPE_DOUBLE);
        realTime_goto(0,60);
        background_WaitFor(EVENTS_MOTOR_OFF);
        barCode = realTime_barcode_read();
        sendCommand(CMD_CBARRE_READ_R, 2, (char *)&barCode);
        if (barCode != -1)
            realTime_goto(-10,75);
        background_WaitFor(EVENTS_MOTOR_OFF);
    }
    
    
    //Scan single barcode
    mission_status = MISSION_DONE;
    for(i=0;i<5;i++){
        listBarcode[i]= -1;
        if (barCode & (1<<i)){
            while(listBarcode[i]==-1) {
                realTime_goto(barCodeStartPosition[i].x,barCodeStartPosition[i].y);
                background_WaitFor(EVENTS_MOTOR_OFF);
                realTime_barcode_arm(BARCODE_TYPE_SINGLE);
                realTime_goto(barCodeStartPosition[i].x-LENGHTBARCODE,barCodeStartPosition[i].y);
                background_WaitFor(EVENTS_MOTOR_OFF);
                listBarcode[i]= realTime_barcode_read();
                if (listBarcode[i] == 7)
                    listBarcode[i] = 2;
                if(listBarcode[i]==-1)
                    mission_status=MISSION_FAIL;
                sendCommand(CMD_CBARRE_READ_R, 2, (char *)&listBarcode[i]);
            }
        }
    }
    
    
    //Distribution y position
    realTime_goto(-140,Y_BARCODE);
    background_WaitFor(EVENTS_MOTOR_OFF);        
    realTime_goto(X_DISTRIBUTION,Y_DISTRIBUTION);
    background_WaitFor(EVENTS_MOTOR_OFF);
    
    int egg;
    int position;
    for(egg=0;egg<NEGGS;egg++) {
        position=0;
        while (position<NPOSITIONS && listBarcode[position]!=listEggs[egg])
            position++;
        if (position < NPOSITIONS) {
            // Distribute
            listBarcode[position]=-1;
            realTime_goto(listCoordDistri[position].x,listCoordDistri[position].y);
            background_WaitFor(EVENTS_MOTOR_OFF);
            distribution(BASKET);
       } else {
            // Trash
            distribution(TRASH);
        }
    }
    // Disable turntable regulation
    realtime_turntable_disable();
}
    
    // Send Mission complete/failed message

    //sendCommand(CMD_GO_R, 1,&mission_status);
*/
