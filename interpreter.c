/* 
 * File:   interpreter.c
 * Author: Bruno Cornil
 *
 * Created on 1 décembre 2015, 15:46
 */

#include <stdlib.h>
#include <string.h>

#include "../PrcLib/PRC.h"		//  PRC.h contains the prototype of PrcInit() and timing functions
#include "../PrcLib/LEDs.h"		// Contains the prototypes of LEDs functions
#include "../PrcLib/DigIn.h"	// Contains the prototypes of digital inputs functions
#include "../PrcLib/anaIn.h"	// Contains the prototypes of analog inputs functions
#include "../PrcLib/motors.h"	// Contains the prototypes of motors functions

#include "robot.h"
#include "interpreter.h"
#include "protocol.h"           //  protocol.h contains the prototypes of protocol interface functions
#include "realTimeTasks.h"
#include "mission.h"

// Global Variables
int log_mask = 0;

/* -----------------------------------------------------------------------------
 * -- 'Set' functions ----------------------------------------------------------
 * -----------------------------------------------------------------------------
 */

/*
 * Set Motor, params = (enable, type, speed)
 *   enable = 0/off or 1/on
 *   type   = 0/move or 1/rotate
 *   speed  = -2048 -> 2047
 */
void motorSet(unsigned char *command) {                
    int motorIsEnable;
    int motorMoveType;
    int motorVoltage;
    
    motorIsEnable = command[1] & 0x1;
    motorMoveType = command[2] & 0x1;
    motorVoltage = (command[4] << 8) + command[3];
    
    if (motorIsEnable == 1) {
        motorSetVoltage(0,motorVoltage);
        if (motorMoveType == 0) {
            motorSetVoltage(1,-motorVoltage);
        } else {
            motorSetVoltage(1,motorVoltage);
        }
        motorEnable(WHEEL_LEFT);
        motorEnable(WHEEL_RIGHT);	
    } else {
        motorDisable(WHEEL_LEFT);
        motorDisable(WHEEL_RIGHT);
    }
}

/*
 * Move robot (in cm)
 */
void robot_move(unsigned char *command) {
    float distance;
    
    memcpy(&distance, command+1, 4);  
    realTime_move(distance);
}

/*
 * Rotate robot (in deg)
 */
void robot_rotate(unsigned char *command) {
    float angle;
    
    memcpy(&angle, command+1, 4);   
    realTime_rotate(pi/180 *angle);
}

/*
 * Move robot to coordinates x, y (in cm)
 */
void robot_goto(unsigned char *command) {
    float x, y;
    
    memcpy(&x, command+1, 4);
    memcpy(&y, command+5, 4);  
    realTime_goto(x,y);
}

/* -----------------------------------------------------------------------------
 * -- 'Get' functions ----------------------------------------------------------
 * -----------------------------------------------------------------------------
 */

/*
 * Read motor speed, params = (motor_id)
 *   motor_id = 0 or 1
 */
void report_motor_speed(unsigned char *command){
    int motor_id;
    int speed;
    char data[2];

    motor_id=command[1] & 0x1; // make sure id is 0 or 1
    
    //speed = motor_speed[motor_id];
    speed = realtime_motor_speed(motor_id);
    data[0] =(speed & 0xff);
    data[1] = speed >> 8;
    sendCommand(CMD_MOTOR_SPEED_READ_R,2,data);
}

/*
 * Read motor encoder, params = (motor_id)
 *   motor_id = 0 or 1
 */
void report_motor_encoder(unsigned char *command){
    int i;
    int motor_id;
    long value;
    char data[4];
    
    motor_id=command[1] & 0x1; // make sure id is 0 or 1
    
    value = encoderRead(motor_id);
    for (i=0;i<4;i++){
        data[i]=(value & 0xff);
        value = value >> 8;
    }
    sendCommand(CMD_MOTOR_ENC_READ_R,4,data);
}

/*
 * Read analog encoder, params = (encoder_id)
 *   encoder_id = 0 or 1
 */
void report_encoder(unsigned char *command){
    int encoder_id;
    int value;
    char data[2];
    
    encoder_id=command[1] & 0x1; // make sure id is 0 or 1
    
    value = anaInRead(encoder_id);
    data[0] =(value & 0xff);
    data[1] = value >> 8;
    sendCommand(CMD_ENC_READ_R, 2,data);
}

/*
 * Report barcode value
 */
void report_barcode() {
    char data;

    data = (char)(realTime_barcode_read() & 0xff);
    sendCommand(CMD_CBARRE_READ_R, 1,&data);
}

/* -----------------------------------------------------------------------------
 * -- Command Interpreter ------------------------------------------------------
 * -----------------------------------------------------------------------------
 */

/*
 * Initialize protocol buffer
 */
void initCommandInterface(void) {
    initProtocolBuffer();
}

/*
 * Command Interpreter
 */
void processCommand(void) {
    unsigned char *command;

    command = getCommand();
    if (command != NULL) {
        switch(command[0]){
            case CMD_WRITE_LED_ALL:
                ledWriteByte(command[1]);
                break;
            case CMD_MOTOR_SET:
                motorSet(command);
                break;
            case CMD_MOTOR_SPEED_READ:
                report_motor_speed(command);
                break;    
            case CMD_CBARRE_SCAN:
                realTime_barcode_arm(command[1]);
                break;
            case CMD_CBARRE_READ:
                report_barcode();
                break;
            case CMD_ENC_READ:
                report_encoder(command);
            case CMD_MOVE:
                robot_move(command);
                break;
            case CMD_ROTATE:
                robot_rotate(command);
                break;
            case CMD_GOTO:
                robot_goto(command);
                break;
            case CMD_RT_LOG:
                realTime_setLog(command[1]);
                break;
            case CMD_GO:
                isStartMission = 1;
                break;
            case CMD_ODO_RESET:
                realTime_odometerReset();
                break;
            case CMD_DEBUG:
                break;
                        
        }
    }
}


