/*
 * common.h
 *
 * Created: 7/20/2015 8:10:05 PM
 *  Author: Vo Huu Tai
 */ 


#ifndef COMMON_H_
#define COMMON_H_
//include common library
#include <avr/io.h>			
#include <avr/interrupt.h>	
#ifndef F_CPU
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdbool.h>
//define common macro
#endif
#define cbi(port, bit) (port) &= ~(1 << (bit))		// set bit
#define sbi(port, bit) (port) |=  (1 << (bit))		// clear bit
#define ibi(port,bit) (port) ^= (1 << (bit))		// invert bit

//Led debug register
#define DDR_LED_BUG		DDRB
#define PORT_LED_BUG	PORTB
#define PIN_LED_BUG		7

#define LED_BUG_ON		cbi(PORT_LED_BUG,PIN_LED_BUG)					// Turn on Led debug
#define LED_BUG_OFF		sbi(PORT_LED_BUG,PIN_LED_BUG)					// Turn off Led debug
#define LED_BUG_BLINK	ibi(PORT_LED_BUG,PIN_LED_BUG)					// Blink led debug

#define LED_BUB_INIT sbi(DDR_LED_BUG,PIN_LED_BUG);\
					 sbi(PORT_LED_BUG,PIN_LED_BUG)

//Switch and Key
#define DDR_KEY		DDRB
#define PORT_KEY	PORTB
#define PIN_KEY		PINB
#define KEY1		0
#define KEY2		1
#define KEY3		2
// it means input, and use pull up resistor
#define KEY_INIT	cbi(DDR_KEY,KEY1);\
					sbi(PORT_KEY,KEY1);\
					cbi(DDR_KEY,KEY2);\
					sbi(PORT_KEY,KEY2);\
					cbi(DDR_KEY,KEY3);\
					sbi(PORT_KEY,KEY3)

#define DDR_SWITCH	DDRC
#define PORT_SWITCH PORTC
#define PIN_SWITCH	PINC
#define SW1		3
#define SW2		2
#define SW3		1
#define SW4		0

//input and use pull up resistor
#define SWITCH_INIT	cbi(DDR_SWITCH,SW1);\
					sbi(PORT_SWITCH,SW1);\
					cbi(DDR_SWITCH,SW2);\
					sbi(PORT_SWITCH,SW2);\
					cbi(DDR_SWITCH,SW3);\
					sbi(PORT_SWITCH,SW3);\
					cbi(DDR_SWITCH,SW4);\
					sbi(PORT_SWITCH,SW4)
#endif /* COMMON_H_ */