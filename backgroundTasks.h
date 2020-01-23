/* 
 * File: backgroundTasks.h
 * Author: Bruno Cornil
 *
 * Created on 1 décembre 2015, 15:27
 */

#ifndef BACKGROUND_TASKS_H
#define	BACKGROUND_TASKS_H

void background_Init(void);
void background_SetTimer(int sleepTime);
void background_WaitFor(int);

#define EVENTS_NOTHING 0
#define EVENTS_TIMER     (1 << 0)
#define EVENTS_MOTOR_OFF (1 << 1)
#define EVENTS_ODO_RESET (1 << 2)
#define EVENTS_TURN_OFF  (1 << 3)

extern int background_events;

#endif	/* BACKGROUND_TASKS_H */