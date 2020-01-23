/* 
 * File: command.h  
 * Author: Bruno Cornil
 *
 * Created on 1 décembre 2015, 15:27
 */

#ifndef PROTOCOL_H
#define	PROTOCOL_H

void initProtocolBuffer(void);
unsigned char* getCommand(void);
void sendCommand(char command, int size, char *data);

#endif	/* PROTOCOL_H */

