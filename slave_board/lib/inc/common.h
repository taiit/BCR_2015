/*
 * common.h
 *
 * Created: 7/25/2015 9:39:13 PM
 *  Author: Vo Huu Tai
 */ 


#ifndef COMMON_H_
#define COMMON_H_
//include common library
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#ifndef F_CPU
#define F_CPU 12000000UL
#include <util/delay.h>
#include <stdbool.h>
//
//define common macro
//
#endif
#define cbi(port, bit) (port) &= ~(1 << (bit))		// set bit
#define sbi(port, bit) (port) |=  (1 << (bit))		// clear bit
#define ibi(port,bit) (port) ^= (1 << (bit))		// invert bit
#define PUBLIC
#define PRIVATE
//
//Beep
//
#define DDR_BEEP		DDRD
#define PORT_BEEP		PORTD
#define PIN_BEEP		4
#define BEEP_INIT		sbi(DDR_BEEP,PIN_BEEP);\
						cbi(PORT_BEEP,PIN_BEEP)
//
//Led 7 SEG
//
#define DDR_LED7	DDRB
#define PORT_LED7	PORTB
#define LATCH		2
#define DATA		4
#define SCK			5

#define LED7_INIT	DDR_LED7	|= (1<<LATCH) | (1<<SCK) |(1<<DATA);\
					PORT_LED7	|= (1<<LATCH) | (1<<SCK) | (1<<DATA)
#endif /* COMMON_H_ */