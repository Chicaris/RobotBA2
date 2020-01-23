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

#include "../PrcLib/PRC.h"
#include "../PrcLib/LEDs.h"		// Contains the prototypes of LEDs functions
#include "../PrcLib/DigIn.h"	// Contains the prototypes of digital inputs functions
#include "../PrcLib/anaIn.h"	// Contains the prototypes of analog inputs functions
#include "../PrcLib/motors.h"	// Contains the prototypes of motors functions

#include "robot.h"              //  robot.h contains main project defines
#include "realTimeTasks.h"

#define MIN(a,b) (((a)<(b))?(a):(b)) 
#define MAX(a,b) (((a)>(b))?(a):(b))

#define RTLOG_QUEUE_SIZE 32

float pi;
int  reset_position=0;
int  realTime_Control=0;
int  realTime_Log=0;


struct sEncoder {
	long current_value;
	long previous_1s_value;
	long target_position;
};

struct {
    int dir;
    int speed;
    struct sEncoder encoder;
} motor[2];

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
    long  encoder_previous_value[2];
} odometer;

struct {
    long encoder_target;
    int  isMoving;
} turntable;

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

void realTime_odometerReset(void) {
    realTime_Control |= RTCTRL_ODO_RESET_MASK;
}

int realTime_isOdometerResetRequest(void) {
    return(realTime_Control & RTCTRL_ODO_RESET_MASK);
}

int realTime_isMoving(void){
    return(move.state || (realTime_Control & RTCTRL_MOTOR_ENABLE_MASK));
}

int realtime_motor_speed(int id) {
    return motor[id].speed;
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
    int dir;
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
    dir=1;
    if (delta_angle > 2.3){
        delta_angle -= pi;
        dir =-1;
    }
    if (delta_angle < -2.3){
        delta_angle += pi;
        dir = -1;
    }

    encoder_distance = delta_angle / scaling.rotation; 
            
    // Set wheel positions for rotation ...
    move.rotation = encoder_distance;    
    
    // ... then translate ...
    encoder_distance = sqrt((odometer.x-target_x)*(odometer.x-target_x)+(odometer.y-target_y)*(odometer.y-target_y));
    encoder_distance /= scaling.translation;
    // Set wheel positions           
    move.translation = dir*encoder_distance;    
    
 
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
    int elem1, elem2;
    long start_position, interval, samplePoint;
} barcode;

int elementA[] = {0x0d,0x19,0x13,0x3d,0x23,0x31,0x2f,0x3b,0x37,0x0b};
int elementC[] = {0x72,0x66,0x6c,0x42,0x5c,0x4e,0x50,0x44,0x48,0x74};
int elementB[] = {0x27,0x33,0x1b,0x21,0x1d,0x39,0x05,0x11,0x09,0x17};

/*
 * Convert ElementA/B/C into a code
 */
int realTime_barcode_getCode(int element) {
    int rv, i;
    rv=-1;
    for(i=0;i<10;i++) {
        if( (elementA[i]==element) || (elementB[i]==element) || (elementC[i]==element)) {
            rv=i;
            break;
        }
    }
    return rv;
}

/*
 *  Arm barcode acquisition
 */
void realTime_barcode_arm(int type) {
    barcode.state = BARCODE_STATE_SEARCH_HEADER;
    barcode.type = type;
    barcode.elem1 = 0;
    barcode.elem2 = 0;
    realTime_Control |= RTCTRL_BARCODE_ENABLE_MASK;
}

/*
 *  Return barcode value (or -1 if not available/error)
 */
int  realTime_barcode_read(void) {
    int rv, code;
    
    rv=-1;
    if (barcode.state == BARCODE_STATE_IDLE) {
        rv = realTime_barcode_getCode(barcode.elem1);
        if ((rv != -1) && (barcode.type == BARCODE_TYPE_DOUBLE)) {
            code = realTime_barcode_getCode(barcode.elem2);
            if (code != -1) {
                rv = rv*10+code;
            }
        }
    }
    return rv;
}

void realTime_barcode_process(int level, long current_position){
    
    static long oldLog = 0;
    long logValue;
    
    switch (barcode.state) {
        case BARCODE_STATE_SEARCH_HEADER:
            if (level == 1) {
                barcode.start_position = current_position;
                barcode.prevLevel = 1;
                barcode.counter = 3;
                barcode.state = BARCODE_STATE_PROCESS_HEADER; 
            }
            break;
            
        case BARCODE_STATE_SEARCH_GUARD:            
            if (level == 1) {
                barcode.start_position = current_position;
                barcode.prevLevel = 1;
                barcode.counter = 4;
                barcode.state = BARCODE_STATE_PROCESS_GUARD;
            }
            break;       
            
        case BARCODE_STATE_PROCESS_HEADER:
            if (level != barcode.prevLevel) {
                barcode.prevLevel=level;
                barcode.counter--;
                if (barcode.counter == 0) {
                    barcode.interval = (current_position - barcode.start_position)/3;
                    barcode.samplePoint = current_position + (barcode.interval >> 1);
                    barcode.counter = 8;
                    barcode.state = BARCODE_STATE_GET_ELEM_A;
                }
            }
            
        case BARCODE_STATE_PROCESS_GUARD:    
            if (level != barcode.prevLevel) {
                barcode.prevLevel=level;
                barcode.counter--;
                if (barcode.counter == 0) {
                    barcode.interval = (current_position - barcode.start_position)>>2;
                    barcode.samplePoint = current_position + (barcode.interval >> 1);
                    barcode.counter = 8;
                    barcode.state = BARCODE_STATE_GET_ELEM_C;
                }
            }
            break;
        case BARCODE_STATE_GET_ELEM_A:
        case BARCODE_STATE_GET_ELEM_C:
            if (current_position >= barcode.samplePoint) {
                barcode.samplePoint += barcode.interval;
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
                        barcode.elem1 = (barcode.elem1 << 1) + (level & 0x1);
                    else
                        barcode.elem2 = (barcode.elem2 << 1) + (level & 0x1);
                }
            }
            break;  
    }
    // RT Logs
    logValue = ((current_position & 0xff) << 24) + ((barcode.samplePoint & 0xff) << 16) + ((barcode.counter & 0xf) << 12) + ((barcode.interval & 0xff) << 4) + ((barcode.state & 0x7) << 1) + (level & 0x1);
    if ((realTime_Log & RTLOG_FAST_BARCODE_MASK) && (oldLog != logValue) && realTime_isMoving()) {
        realTime_LogPush(RTLOG_FAST_BARCODE, &logValue);
        oldLog = logValue;
    }
}

/*******************************************************************************
 * Turntable processing
 ******************************************************************************/

int realTime_isTurntableMoving(void){
    return turntable.isMoving;
}

        
void realtime_turntable_enable(void){
    turntable.encoder_target = anaInRead(ENCODER_TURN);
    turntable.isMoving = 0;
    motorSetVoltage(MOTOR_TURN,0);
    motorEnable(MOTOR_TURN);
    realTime_Control |= RTCTRL_TURNTABLE_ENABLE_MASK;
}

void realtime_turntable_disable(void){
    realTime_Control &= ~RTCTRL_TURNTABLE_ENABLE_MASK;
    motorSetVoltage(MOTOR_TURN, 0);
    motorDisable(MOTOR_TURN);
}

void realtime_turntable_next(void){
    turntable.encoder_target = (turntable.encoder_target + MOTOR_TURN_ANGLE) % 1024;
    turntable.isMoving = 1;
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

    long position_error[2];
    long voltage[2];
    long mean_error, diff_error;
    long turntable_error;
    int  odometer_move[2];
    int  isOnTarget;
    int  barCodeLevel;
    float distance;
    int i;

    // Read wheel positions
    for (i=0;i<2;i++)
        if (motor[i].dir==MOTOR_REVERSE)
            motor[i].encoder.current_value = -encoderRead(i);
        else
            motor[i].encoder.current_value = encoderRead(i);

    // Control loop runs every 1ms
    if (realTime_Control & RTCTRL_MOTOR_ENABLE_MASK) {
        // Compute position error for each wheel(distance between target and current position)
        for (i=0;i<2;i++)
            position_error[i] = motor[i].encoder.target_position - motor[i].encoder.current_value;

        // Compute mean error (regulation) and differential error (speed compensation)
        mean_error = (abs(position_error[WHEEL_LEFT])+abs(position_error[WHEEL_RIGHT])) >> 1;
        diff_error = (abs(position_error[WHEEL_LEFT])-abs(position_error[WHEEL_RIGHT])) >> 1;

        // Slow down if a scan is requested
        if((realTime_Control & RTCTRL_BARCODE_ENABLE_MASK)!=0)
            voltage[WHEEL_LEFT]  = MIN(MOTOR_VMAX_SCAN, MOTOR_VOLTAGE_OFFSET + KLOOP * mean_error);
        else
            voltage[WHEEL_LEFT]  = MIN(MOTOR_VMAX, MOTOR_VOLTAGE_OFFSET + KLOOP * mean_error);
        voltage[WHEEL_RIGHT] = voltage[WHEEL_LEFT];

        // Slow down fastest wheel
        if (diff_error < 0)
            voltage[WHEEL_LEFT]  += KLOOP * diff_error;
        else
            voltage[WHEEL_RIGHT] -= KLOOP * diff_error;
        
        // Update motors
        for (i=0;i<2;i++) {
            if (position_error[i] < 0) 
                voltage[i] = -voltage[i];
            if (motor[i].dir==MOTOR_REVERSE)
                motorSetVoltage(i, -voltage[i]);
            else
                motorSetVoltage(i, voltage[i]);
	    }
	}

    // Barcode reader
    if((realTime_Control & RTCTRL_BARCODE_ENABLE_MASK)!=0) {
        barCodeLevel = digInRead(BARCODE_SENSOR);  
        realTime_barcode_process(barCodeLevel, ((motor[0].encoder.current_value + motor[1].encoder.current_value) >> 1) );
    }
    
    // Turntable
    long turn_voltage;
    if((realTime_Control & RTCTRL_TURNTABLE_ENABLE_MASK)!=0) {
        turntable_error=turntable.encoder_target-anaInRead(ENCODER_TURN);
        // FIXME : Better limit ....
        if (turntable_error < -512)
            turntable_error += 1024;
        if (turntable_error > 512)
            turntable_error -= 1024;
        if (abs(turntable_error) < MOTOR_TURN_MAX_ERROR)
            turntable.isMoving=0;
        if (turntable_error > 0)
            turn_voltage = MIN(MOTOR_VMAX, MOTOR_TURN_OFFSET + KLOOP_TURN * turntable_error);
        else
            turn_voltage = MAX(-MOTOR_VMAX, -MOTOR_TURN_OFFSET + KLOOP_TURN * turntable_error);
        motorSetVoltage(MOTOR_TURN, turn_voltage);

    }
           
    // Odometry runs every 10ms 
    counter_10ms++;
    if (counter_10ms == 10) {
        counter_10ms=0;
        // Odometry
        for (i=0;i<2;i++) {
            odometer_move[i] = (int)(motor[i].encoder.current_value - odometer.encoder_previous_value[i]);
            odometer.encoder_previous_value[i]=motor[i].encoder.current_value;
        }

        if((realTime_Control & RTCTRL_ODO_RESET_MASK)!=0) {
            realTime_Control &=  ~RTCTRL_ODO_RESET_MASK;
            odometer.angle = pi/2.0;
            odometer.x = 0.0;
            odometer.y = 0.0;

        } else {
            distance        = scaling.translation * (float)(odometer_move[0]+odometer_move[1])/2.0;
            odometer.x     += distance * cos(odometer.angle);
            odometer.y     += distance * sin(odometer.angle);           
            odometer.angle += scaling.rotation    * (float)(odometer_move[1]-odometer_move[0])/2.0;
        }
        
        // RT Logs
        if (realTime_Log & RTLOG_FAST_ENCODER_0_MASK)
            realTime_LogPush(RTLOG_ENCODER_0, &motor[0].encoder.current_value);
        if (realTime_Log & RTLOG_FAST_ENCODER_1_MASK)
            realTime_LogPush(RTLOG_ENCODER_1, &motor[1].encoder.current_value);
    }
    
    // Run every 1s 
	counter_1s++;
    if (counter_1s == 1000) {
        counter_1s=0;
        ledToggle(LED_ALIVE);       
        
        // Compute speed (averaged over 1s)
        for (i=0;i<2;i++){
            motor[i].speed= (int)(motor[i].encoder.current_value-motor[i].encoder.previous_1s_value);
            motor[i].encoder.previous_1s_value = motor[i].encoder.current_value;
        }

        // A move has been requested but didn't started yet
        if ((move.state != 0) && !(realTime_Control & RTCTRL_MOTOR_ENABLE_MASK)) {
            motorSetVoltage(WHEEL_LEFT, 0); motorSetVoltage(WHEEL_RIGHT, 0);
            motorEnable(WHEEL_LEFT); motorEnable(WHEEL_RIGHT); 
            realTime_Control |= RTCTRL_MOTOR_ENABLE_MASK;
        }
        
        // Did wheel reach target position ?
        isOnTarget = 1;
        for (i=0;i<2;i++)
            // Target reached if within +/- POSITION_MAX_ERROR
            if (abs(motor[i].encoder.target_position-motor[i].encoder.current_value) > POSITION_MAX_ERROR)
                isOnTarget = 0;
                   
        // Rotation goes first ...
        if (move.state & MOVE_STATE_ROTATION_MASK) {
            // Define new target position
            motor[WHEEL_LEFT].encoder.target_position -=move.rotation;
            motor[WHEEL_RIGHT].encoder.target_position+=move.rotation;
            move.state &= ~MOVE_STATE_ROTATION_MASK;
        // ... then translation ...
        } else if (move.state & MOVE_STATE_TRANSLATION_MASK) {
            if (isOnTarget) {
                // Define new target position
                motor[WHEEL_LEFT].encoder.target_position +=move.translation;
                motor[WHEEL_RIGHT].encoder.target_position+=move.translation;
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
            realTime_LogPush(RTLOG_ENCODER_0, &motor[0].encoder.current_value);
        if (realTime_Log & RTLOG_ENCODER_1_MASK)
            realTime_LogPush(RTLOG_ENCODER_1, &motor[1].encoder.current_value);
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
    
    motor[WHEEL_LEFT].dir=WHEEL_LEFT_DIR;
    motor[WHEEL_RIGHT].dir=WHEEL_RIGHT_DIR;
    
    odometer.x = 0.0;
    odometer.y = 0.0;
    odometer.angle = pi/2.0;
    
    scaling.translation = WHEEL_RADIUS * pi/180.0;
    scaling.rotation    = scaling.translation/(WHEEL_DISTANCE/2.0);
        
    for (i=0;i<2;i++){
        if (motor[i].dir == MOTOR_REVERSE)
            motor[i].encoder.previous_1s_value = -encoderRead(i);
        else
            motor[i].encoder.previous_1s_value = encoderRead(i);
        odometer.encoder_previous_value[i] = motor[i].encoder.previous_1s_value;
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

