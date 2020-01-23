/* 
 * File:   RADIO.h
 * Author: Kevin
 *
 * Created on 23 avril 2014, 14:23
 */

#ifndef RADIO_H
#define	RADIO_H

/**
 * @brief Initialise la connexion Bluetooth au travers d'un RADIO
 * Attention : cette fonction est bloquante, elle ne se termine que lorsque la connexion est établie avec le PC
 */

typedef enum {
	RADIO_UNINITIALIZED,
	RADIO_NOT_CONNECTED,
	RADIO_CONNECTED
} radioStatusType;

radioStatusType radioGetStatus(void);
radioStatusType radioInit(void);
int radioGetRxBufferSpace(void);
int radioGetTxBufferSpace(void);
char radioGetChar(void);
int radioGetInt(void);
float radioGetFloat(void);
void radioSendChar(char data);
void radioSendInt(int data);
void radioSendFloat(float data);

#endif	/* RADIO_H */
