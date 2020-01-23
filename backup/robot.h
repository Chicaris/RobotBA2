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

#define MOTOR_VMAX      2047
#define MOTOR_FORWARD   1
#define MOTOR_REVERSE  -1

// Robot configuration
#define WHEEL_LEFT      0
#define WHEEL_LEFT_DIR  MOTOR_REVERSE
#define WHEEL_LEFT_VOLTAGE_OFFSET 1200

#define WHEEL_RIGHT     1
#define WHEEL_RIGHT_DIR MOTOR_FORWARD
#define WHEEL_RIGHT_VOLTAGE_OFFSET 1200

#define WHEEL_RADIUS    4.08
#define WHEEL_DISTANCE  16
        
// Regulation
#define KLOOP 30

#define POSITION_MAX_ERROR 1

#endif	/* ROBOT_H */

