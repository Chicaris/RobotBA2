/* 
 * File:   main.c
 * Author: Bruno
 *
 * Created on 23 novembre 2015, 21:12
 */

#include <stdio.h>
#include <stdlib.h>

#include "../PrcLib/PRC.h"		//  PRC.h contains the prototype of PrcInit() and timing functions
#include "../PrcLib/DigIn.h"
#include "../PrcLib/AnaIn.h"
#include "../PrcLib/AMB2300.h"	//  Radio.h contains the prototypes of Bluetooth radio functions

#include "robot.h"
#include "backgroundTasks.h"    //  backgroundTasks.h contains the prototypes of background tasks functions/structures
#include "realTimeTasks.h"      //  realTimeTasks.h contains the prototypes of realtime tasks functions/structures
#include "mission.h"

/*
 * Program entry point
 */
int main(int argc, char** argv) {

    // Initialize PRC functions
	prcInit();
    // Initialize Analog interfaces
    digInInit();
    //anaInInit();
    // Initialize Bluetooth Interface
	radioInit();
    // Initialize RealTime and Background tasks
    background_Init();
    realTime_Init();
    
	// Main loop
	while(1) {
        background_WaitFor(EVENTS_NOTHING);
        if(isStartMission){
            start_mission();
            isStartMission=0;
        }
    }
    
    return (EXIT_SUCCESS);
}


