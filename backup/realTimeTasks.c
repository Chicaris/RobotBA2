/* 
 * File:   realTimeTasks.c
 * Author: Bruno Cornil
 *
 * Created on 1 décembre 2015, 15:17
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <xc.h>

#include "../PrcLib/LEDs.h"		// Contains the prototypes of LEDs functions
#include "../PrcLib/DigIn.h"	// Contains the prototypes of digital inputs functions
#include "../PrcLib/anaIn.h"	// Contains the prototypes of analog inputs functions
#include "../PrcLib/motors.h"	// Contains the prototypes of motors functions

#include "robot.h"              //  robot.h contains main project defines
#include "realTimeTasks.h"

#define RTLOG_QUEUE_SIZE 32

float pi;
int  reset_position=0;
int  realTime_Control=0;
int  realTime_Log=0;

int  motor_speed[2];
long motor_encoder_prev[2];
long encoder_target_position[2];

struct {
    int dir;
    int voltageOffset;
} motorParams[2];

struct {
    float rotation;
    float translation;
} scaling;

#define MOVE_STATE_ROTATION_MASK    (1<<0)
#define MOVE_STATE_TRANSLATION_MASK (1<<1)
struct {
    int  state;
    long rotation;
    long translation;
} move;

struct {
    float x;
    float y;
    float angle;
    long  lastEncoderValue[2];
} odometer;

/* -----------------------------------------------------------------------------
 * -- Real Time queues functions -----------------------------------------------
 * -----------------------------------------------------------------------------
 */

/*
 * Queue data structures
 *   Queue empty -> logRT_wrAdd == logRT_rdAdd
 *   Queue full  -> logRT_wrAdd+1 == logRT_rdAdd
 */
struct sLogQueue logRT_queue[RTLOG_QUEUE_SIZE];
int logRT_wrAdd=0;
int logRT_rdAdd=0;
int logRT_isFull=0;

/*
 * Pull a log from the RT log queue
 */
int realTime_LogPull(struct sLogQueue *logEntry){
    int rv=0;
    int rdAdd;
    
    rdAdd = logRT_rdAdd;
    // If queue not empty
    if (logRT_wrAdd != rdAdd) {
        rv=1;
        *logEntry = logRT_queue[rdAdd];
        rdAdd++;
        if (rdAdd >= RTLOG_QUEUE_SIZE)
            rdAdd = 0;
        // Update read address when data is transfered
        logRT_rdAdd = rdAdd;
    }
    return rv;
}
/*
 * Push a new log into the RT log queue 
 */
void realTime_LogPush(char id, long *value) {
    int wrAdd;
    
    wrAdd = logRT_wrAdd+1;
    if (wrAdd >= RTLOG_QUEUE_SIZE)
        wrAdd=0;
    // If queue not full
    if (wrAdd != logRT_rdAdd) {
        if (logRT_isFull == 1)
            id |= RTCTRL_LOG_OVERFLOW;
        logRT_isFull=0;
        
        logRT_queue[logRT_wrAdd].id = id;
        logRT_queue[logRT_wrAdd].value = *value;
        
        // Update write address only when data are completely written 
        logRT_wrAdd=wrAdd;
    } else {
        logRT_isFull=1;
    }
}

/* -----------------------------------------------------------------------------
 * -- Real Time interface functions --------------------------------------------
 * -----------------------------------------------------------------------------
 */

void realTime_setLog(int log){
    realTime_Log = log ;
}

void realTime_resetOdometer(void) {
    realTime_Control |= RTCTRL_ODO_RESET_MASK;
}

int realTime_isMoving(void){
    // True = move.state != 0
    return (move.state);
}

/*
 * Move Robot by <distance> cm
 */
void realTime_move(float distance)
{
    // Convert distance in encoder steps       
    move.rotation    = 0;
    move.translation = distance / scaling.translation;    
    // Request a move
    move.state |= MOVE_STATE_TRANSLATION_MASK;    
}

/*
 * Rotate Robot by <angle> rd
 */
void realTime_rotate(float angle)
{
    // Convert angle in encoder steps       
    move.rotation    = angle / scaling.rotation;
    move.translation = 0;
    // Request a move
    move.state |= MOVE_STATE_ROTATION_MASK;    
}
/*
 * Move Robot from current to target position
 *   Setup rotation then translation
 *   NOTE : realTime_isMoving should return false before calling this function
 */
void realTime_goto(float target_x, float target_y){
    float current_angle, target_angle, delta_angle;
    float encoder_distance;
    
    // First rotate ...
    // Bound odemeter angle to [0:2*pi]
    current_angle=atan(tan(odometer.angle));
    if (cos(odometer.angle) < 0)
        current_angle+=pi;
    
    // Compute target angle [0:2*pi]
    target_angle = atan((target_y-odometer.y)/(target_x-odometer.x));
    if ((target_x-odometer.x) < 0)
        target_angle+=pi;
    
    // Compute required rotation
    delta_angle=target_angle-current_angle;
    if (delta_angle > pi)
        delta_angle-=2*pi;
    if (delta_angle < -pi)
        delta_angle+=2*pi;
    
    encoder_distance = delta_angle / scaling.rotation; 
            
    // Set wheel positions for rotation ...
    move.rotation = encoder_distance;    
    
    // ... then translate ...
    encoder_distance = sqrt((odometer.x-target_x)*(odometer.x-target_x)+(odometer.y-target_y)*(odometer.y-target_y));
    encoder_distance /= scaling.translation;
    // Set wheel positions           
    move.translation = encoder_distance;    
            
    // ... then request a move
    move.state = MOVE_STATE_ROTATION_MASK | MOVE_STATE_TRANSLATION_MASK;
}

/*******************************************************************************
 * Bar code processing
 ******************************************************************************/
#define BARCODE_STATE_IDLE           0
#define BARCODE_STATE_SEARCH_HEADER  1
#define BARCODE_STATE_PROCESS_HEADER 2
#define BARCODE_STATE_SEARCH_GUARD   3
#define BARCODE_STATE_PROCESS_GUARD  4
#define BARCODE_STATE_GET_ELEM_A     5
#define BARCODE_STATE_GET_ELEM_C     6

struct {
    int state;
    int type;
    int prevLevel;
    int counter;
    int elemA, elemC;
    long start_position, interval, samplePoint;
} barcode;

/*
 *  Arm barcode acquisition
 */
void realTime_barcode_arm(int type) {
    barcode.state = BARCODE_STATE_SEARCH_HEADER;
    barcode.type = type;
    barcode.elemA = 0;
    barcode.elemC = 0;
    realTime_Control |= RTCTRL_BARCODE_ENABLE_MASK;
}

/*
 *  Return barcode value (or -1 if not available/error)
 */
int  realTime_barcode_read(void) {
    int rv;
    if (barcode.state != BARCODE_STATE_IDLE) {
        rv = -1;
    } else {
        rv = barcode.elemA;
        if ( barcode.type == BARCODE_TYPE_DOUBLE)
            rv = rv*10+barcode.elemC;
    }
    return rv;
}

void realTime_barcode_process(int level, long current_position){
    
    long logValue = (long)level;
    
    switch (barcode.state) {
        case BARCODE_STATE_SEARCH_HEADER:
        case BARCODE_STATE_SEARCH_GUARD:
            if (level == 1) {
                barcode.start_position = current_position;
                barcode.prevLevel = 1;
                barcode.counter = 3;
                if (barcode.state == BARCODE_STATE_SEARCH_HEADER)
                    barcode.state = BARCODE_STATE_PROCESS_HEADER;
                else
                    barcode.state = BARCODE_STATE_PROCESS_GUARD;
            }
            break;
        case BARCODE_STATE_PROCESS_HEADER:
        case BARCODE_STATE_PROCESS_GUARD:    
            if (level != barcode.prevLevel) {
                barcode.prevLevel=level;
                barcode.counter--;
                if (barcode.counter == 0) {
                    barcode.interval = (current_position - barcode.start_position)/3;
                    barcode.samplePoint = current_position + (barcode.interval >> 1);
                    barcode.counter = 8;
                    if (barcode.state == BARCODE_STATE_PROCESS_HEADER) {
                        barcode.state = BARCODE_STATE_GET_ELEM_A;
                    } else {
                        barcode.samplePoint += barcode.interval;
                        barcode.state = BARCODE_STATE_GET_ELEM_C;
                    }
                }
            }
            break;
        case BARCODE_STATE_GET_ELEM_A:
        case BARCODE_STATE_GET_ELEM_C:
            if (current_position >= barcode.samplePoint) {
                barcode.samplePoint = current_position + (barcode.interval >> 1);
                barcode.counter--;
                if (barcode.counter == 0) {
                    if ((barcode.type == BARCODE_TYPE_SINGLE) || (barcode.state == BARCODE_STATE_GET_ELEM_C)) {
                        barcode.state = BARCODE_STATE_IDLE;
                        realTime_Control &= ~RTCTRL_BARCODE_ENABLE_MASK;
                    } else { 
                        barcode.state = BARCODE_STATE_SEARCH_GUARD;
                    }
                } else {
                    if (barcode.state == BARCODE_STATE_GET_ELEM_A)
                        barcode.elemA = (barcode.elemA << 1) + (level & 0x1);
                    else
                        barcode.elemC = (barcode.elemC << 1) + (level & 0x1);
                }
            }
            break;  
    }
    // RT Logs
    if ((realTime_Log & RTLOG_FAST_BARCODE_LEVEL_MASK) && (barcode.state != BARCODE_STATE_IDLE) && (realTime_Control & RTCTRL_MOTOR_ENABLE_MASK))
        realTime_LogPush(RTLOG_FAST_BARCODE_LEVEL, &logValue);
}

/* -----------------------------------------------------------------------------
 * -- Real Time process --------------------------------------------------------
 * -----------------------------------------------------------------------------
 */

/*
 * Interrupt/Real Time task (called every 1ms)
 */
void __attribute__ (( interrupt, no_auto_psv )) _T3Interrupt(void) {  
    static unsigned counter_1s=0;
    static unsigned counter_10ms=0;

    long voltage;
    int isOnTarget;
    long encoder_current_value[2];
    int encoderIncrement[2];
    int barCodeLevel;
    float distance;
    int i;

    // Read wheel positions
    for (i=0;i<2;i++)
        if (motorParams[i].dir==MOTOR_REVERSE)
            encoder_current_value[i] = -encoderRead(i);
        else
            encoder_current_value[i] = encoderRead(i);
    
    // Control loop runs every 1ms
    if (realTime_Control & RTCTRL_MOTOR_ENABLE_MASK) {
        for (i=0;i<2;i++) {
            voltage = KLOOP * (encoder_target_position[i]-encoder_current_value[i]);
            // Add offset and clamp voltage to [-2047:2047]]
            if (voltage < 0) {
                voltage -= motorParams[i].voltageOffset;
                if (voltage < -MOTOR_VMAX)
                    voltage = -MOTOR_VMAX;
            }
            if (voltage >= 0) {
                voltage += motorParams[i].voltageOffset;
                if (voltage > MOTOR_VMAX)
                    voltage = MOTOR_VMAX;
            }    
            if (motorParams[i].dir==MOTOR_REVERSE)
                motorSetVoltage(i, -voltage);
            else
                motorSetVoltage(i, voltage);
        }
    }

    // Odometry runs every 10ms 
    counter_10ms++;
    if (counter_10ms == 10) {
        counter_10ms=0;     
        // Odometry
        for (i=0;i<2;i++) {
            encoderIncrement[i] = (int)(encoder_current_value[i] - odometer.lastEncoderValue[i]);
            odometer.lastEncoderValue[i]=encoder_current_value[i];
        }

        if((realTime_Control & RTCTRL_ODO_RESET_MASK)!=0) {
            realTime_Control &=  ~RTCTRL_ODO_RESET_MASK;
            odometer.angle = pi/2.0;
            odometer.x = 0.0;
            odometer.y = 0.0;

        } else {
            distance        = scaling.translation * (float)(encoderIncrement[0]+encoderIncrement[1])/2.0;
            odometer.x     += distance * cos(odometer.angle);
            odometer.y     += distance * sin(odometer.angle);           
            odometer.angle += scaling.rotation    * (float)(encoderIncrement[1]-encoderIncrement[0])/2.0;
        }
        
        // Barcode reader
        if((realTime_Control & RTCTRL_BARCODE_ENABLE_MASK)!=0) {
            barCodeLevel = digInRead(BARCODE_SENSOR);  
            realTime_barcode_process(barCodeLevel, ((encoder_current_value[0] + encoder_current_value[1]) >> 2) );
        }
        // RT Logs
        if (realTime_Log & RTLOG_FAST_ENCODER_0_MASK)
            realTime_LogPush(RTLOG_ENCODER_0, &encoder_current_value[0]);
        if (realTime_Log & RTLOG_FAST_ENCODER_1_MASK)
            realTime_LogPush(RTLOG_ENCODER_1, &encoder_current_value[1]);
    }
    
    // Run every 1s 
	counter_1s++;
    if (counter_1s == 1000) {
        counter_1s=0;
        ledToggle(LED_ALIVE);       
        
        // Compute speed (averaged over 1s)
        for (i=0;i<2;i++){
            motor_speed[i]= (int)(encoder_current_value[i]-motor_encoder_prev[i]);
            motor_encoder_prev[i] = encoder_current_value[i];
        }

        // A move has been requested but didn't started yet
        if ((move.state != 0) && !(realTime_Control & RTCTRL_MOTOR_ENABLE_MASK)) {
            motorSetVoltage(WHEEL_LEFT, 0); motorSetVoltage(WHEEL_RIGHT, 0);
            motorEnable(WHEEL_LEFT); motorEnable(WHEEL_RIGHT); 
            realTime_Control |= RTCTRL_MOTOR_ENABLE_MASK;
        }
        
        // Did we reach target position ?
        isOnTarget = 1;
        for (i=0;i<2;i++)
            // Target reached if within +/- POSITION_MAX_ERROR
            if (abs(encoder_target_position[i]-encoder_current_value[i]) > POSITION_MAX_ERROR)
                isOnTarget = 0;
        
        // Rotation goes first ...
        if (move.state & MOVE_STATE_ROTATION_MASK) {
            // Define new target position
            encoder_target_position[WHEEL_LEFT] -=move.rotation;
            encoder_target_position[WHEEL_RIGHT]+=move.rotation;
            move.state &= ~MOVE_STATE_ROTATION_MASK;
        // ... then translation ...
        } else if (move.state & MOVE_STATE_TRANSLATION_MASK) {
            if (isOnTarget) {
                // Define new target position
                encoder_target_position[WHEEL_LEFT] +=move.translation;
                encoder_target_position[WHEEL_RIGHT]+=move.translation;
                move.state &= ~MOVE_STATE_TRANSLATION_MASK;
            }
        // ... until distance is within max error circle
        } else {
            if (isOnTarget) {
                motorSetVoltage(WHEEL_LEFT, 0); motorSetVoltage(WHEEL_RIGHT, 0);
                motorDisable(WHEEL_LEFT); motorDisable(WHEEL_RIGHT);
                realTime_Control &= ~RTCTRL_MOTOR_ENABLE_MASK;
            }
        }
  
        // RT Logs
        if (realTime_Log & RTLOG_ENCODER_0_MASK)
            realTime_LogPush(RTLOG_ENCODER_0, &encoder_current_value[0]);
        if (realTime_Log & RTLOG_ENCODER_1_MASK)
            realTime_LogPush(RTLOG_ENCODER_1, &encoder_current_value[1]);
        if (realTime_Log & RTLOG_ODO_X_MASK)
            realTime_LogPush(RTLOG_ODO_X, (long *)&odometer.x);
        if (realTime_Log & RTLOG_ODO_Y_MASK)
            realTime_LogPush(RTLOG_ODO_Y, (long *)&odometer.y);
        if (realTime_Log & RTLOG_ODO_A_MASK)
            realTime_LogPush(RTLOG_ODO_A, (long *)&odometer.angle);
    }
    
    // Clear Interrupt flag
    IFS0bits.T3IF = 0;
}


/*
 * Initialize interrupt task, call every 1ms
 */
void realTime_Init() {
    int i;
    
    pi=acos(-1.0);
    reset_position=0;
    
    motorParams[WHEEL_LEFT].dir=WHEEL_LEFT_DIR;   motorParams[WHEEL_LEFT].voltageOffset=WHEEL_LEFT_VOLTAGE_OFFSET;
    motorParams[WHEEL_RIGHT].dir=WHEEL_RIGHT_DIR; motorParams[WHEEL_RIGHT].voltageOffset=WHEEL_RIGHT_VOLTAGE_OFFSET;
    
    odometer.x = 0.0;
    odometer.y = 0.0;
    odometer.angle = pi/2.0;
    
    scaling.translation = WHEEL_RADIUS * pi/180.0;
    scaling.rotation    = scaling.translation/(WHEEL_DISTANCE/2.0);
        
    for (i=0;i<2;i++){
        if (motorParams[i].dir == MOTOR_REVERSE)
            motor_encoder_prev[i] = -encoderRead(i);
        else
            motor_encoder_prev[i] = encoderRead(i);
        odometer.lastEncoderValue[i] = motor_encoder_prev[i];
    }
    
	// Configures timer1 to generate an interrupt every ms
	T3CONbits.TON = 0;		// stop Timer3

	T3CONbits.TCS = 0; 		// Timer3 uses internal clock (Fcy = Fosc/2 = 40MHz)
	T3CONbits.TSIDL = 1; 	// Timer3 is stopped in IDLE mode
	T3CONbits.TCKPS = 0;	// no prescaler
	T3CONbits.TGATE = 0;	// we don't use external enable signal
    
    TMR3 = 0;				// clear timer1
	PR3 = 39999;			// PR3 = period/Tcy - 1 = 1ms/25ns - 1 = 40E3 - 1
//	_T3IP = 4;              // set Timer3 interrupt priority to 4 : 0 (lowest) -> 7 (highest)
	IFS0bits.T3IF = 0;		// clear the interrupt flag
	IEC0bits.T3IE = 1;		// enable the interrupt
    
    T3CONbits.TON = 1;		// start Timer3
}

