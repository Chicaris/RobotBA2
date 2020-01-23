/* 
 * File: robot.h  
 * Author: Bruno Cornil
 *
 * Created on 1 décembre 2015, 15:27
 */
  
#ifndef ROBOT_H
#define	ROBOT_H

// Interfaces
#define LED_RADIO 0
#define LED_ALIVE 7

#define BARCODE_SENSOR  0

#define MOTOR_VMAX      1400
#define MOTOR_VMAX_SCAN 1400
#define MOTOR_FORWARD  1
#define MOTOR_REVERSE   -1
#define MOTOR_VOLTAGE_OFFSET 1300

// Robot configuration
#define WHEEL_LEFT      0
#define WHEEL_LEFT_DIR  MOTOR_REVERSE

#define WHEEL_RIGHT     1
#define WHEEL_RIGHT_DIR MOTOR_FORWARD

#define WHEEL_RADIUS    4.08
#define WHEEL_DISTANCE  18

#define MOTOR_BAND 2
#define MOTOR_BAND_SPEED 2047

#define POSITION_MAX_ERROR 1

// Regulation
#define KLOOP 45

#define MOTOR_TURN 3
#define MOTOR_TURN_OFFSET 400
#define MOTOR_TURN_ANGLE (1024/11)
#define ENCODER_TURN 0
#define KLOOP_TURN 5

#define MOTOR_TURN_MAX_ERROR 10

#endif	/* ROBOT_H */

