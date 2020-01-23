/* 
 * File: realTimeTasks.h
 * Author: Bruno Cornil
 *
 * Created on 1 décembre 2015, 15:27
 */

#ifndef REAL_TIME_TASKS_H
#define	REAL_TIME_TASKS_H

#define RTCTRL_MOTOR_ENABLE  0
#define RTCTRL_MOTOR_ENABLE_MASK (1<<RTCTRL_MOTOR_ENABLE)
#define RTCTRL_ODO_RESET     1
#define RTCTRL_ODO_RESET_MASK (1<<RTCTRL_ODO_RESET)
#define RTCTRL_BARCODE_ENABLE 2
#define RTCTRL_BARCODE_ENABLE_MASK (1<<RTCTRL_BARCODE_ENABLE)
#define RTCTRL_TURNTABLE_ENABLE 3
#define RTCTRL_TURNTABLE_ENABLE_MASK (1<<RTCTRL_TURNTABLE_ENABLE)

#define RTLOG_ENCODER_0 0
#define RTLOG_ENCODER_0_MASK (1<<RTLOG_ENCODER_0)
#define RTLOG_ENCODER_1 1
#define RTLOG_ENCODER_1_MASK (1<<RTLOG_ENCODER_1)
#define RTLOG_ODO_X 2
#define RTLOG_ODO_X_MASK (1<<RTLOG_ODO_X)
#define RTLOG_ODO_Y 3
#define RTLOG_ODO_Y_MASK (1<<RTLOG_ODO_Y)
#define RTLOG_ODO_A 4
#define RTLOG_ODO_A_MASK (1<<RTLOG_ODO_A)
#define RTLOG_FAST_BARCODE 5
#define RTLOG_FAST_BARCODE_MASK (1<<RTLOG_FAST_BARCODE)
#define RTLOG_FAST_ENCODER_0 6
#define RTLOG_FAST_ENCODER_0_MASK (1<<RTLOG_FAST_ENCODER_0)
#define RTLOG_FAST_ENCODER_1 7
#define RTLOG_FAST_ENCODER_1_MASK (1<<RTLOG_FAST_ENCODER_1)

#define RTCTRL_LOG_OVERFLOW (1<<7)

struct sLogQueue {
    char id;
    long value;
};

void realTime_Init(void);
int  realTime_LogPull(struct sLogQueue *logEntry);
void realTime_setLog(int log);

void realTime_odometerReset(void);

void realTime_move(float distance);
void realTime_rotate(float angle);
void realTime_goto(float target_x, float target_y);
int  realTime_isMoving(void);
int  realTime_isOdometerResetRequest(void);

void realtime_turntable(void);
int realTime_isTurntableMoving(void);

#define BARCODE_TYPE_SINGLE 1
#define BARCODE_TYPE_DOUBLE 2
void realTime_barcode_arm(int type);
int  realTime_barcode_read(void);

void realtime_turntable_enable(void);
void realtime_turntable_next(void);
void realtime_turntable_disable(void);

int realtime_motor_speed(int id);

extern float pi;

#endif	/* REAL_TIME_TASKS_H */


