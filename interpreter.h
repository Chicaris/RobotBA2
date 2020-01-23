/* 
 * File:   interpreter.h
 * Author: Bruno Cornil
 *
 * Created on 1 décembre 2015, 15:17
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INTERPRETER_H
#define	INTERPRETER_H

// Command codes
#define CMD_WRITE_LED_ALL       10

#define CMD_MOTOR_SET           20
#define CMD_MOTOR_SPEED_READ    21
#define CMD_MOTOR_SPEED_READ_R  (128+CMD_MOTOR_SPEED_READ)
#define CMD_MOTOR_ENC_READ      22
#define CMD_MOTOR_ENC_READ_R    (128+CMD_MOTOR_ENC_READ)

#define CMD_ENC_READ            30
#define CMD_ENC_READ_R          (128+CMD_ENC_READ)

#define CMD_CBARRE_SCAN         40
#define CMD_CBARRE_SCAN_R       (128+CMD_CBARRE_SCAN)
#define CMD_CBARRE_READ         41
#define CMD_CBARRE_READ_R       (128+CMD_CBARRE_READ)

#define CMD_GOTO                50
#define CMD_MOVE                51
#define CMD_ROTATE              52

#define CMD_GO                  60
#define CMD_GO_R                (128+CMD_GO)

#define CMD_ODO_RESET           70

#define CMD_RT_LOG              100
#define CMD_RT_LOG_R            (128+CMD_RT_LOG)

#define CMD_DEBUG               255

// Command interpreter API
void initCommandInterface(void);
void processCommand(void);

#endif	/* INTERPRETER_H */

