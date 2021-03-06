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
#define F_CPU 12000000UL
#include <util/delay.h>
#include <stdbool.h>
#include <avr/eeprom.h>
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
//Led debug register
//
#define DDR_LED_BUG		DDRB
#define PORT_LED_BUG	PORTB
#define PIN_LED_BUG		7

#define LED_BUG_ON		cbi(PORT_LED_BUG,PIN_LED_BUG)					// Turn on Led debug
#define LED_BUG_OFF		sbi(PORT_LED_BUG,PIN_LED_BUG)					// Turn off Led debug
#define LED_BUG_BLINK	ibi(PORT_LED_BUG,PIN_LED_BUG)					// Blink led debug

#define LED_BUB_INIT sbi(DDR_LED_BUG,PIN_LED_BUG);\
					 sbi(PORT_LED_BUG,PIN_LED_BUG)
//
//Switch and Key
//
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
#define SW_TEST	6

//input and use pull up resistor
#define SWITCH_INIT	cbi(DDR_SWITCH,SW1);\
					sbi(PORT_SWITCH,SW1);\
					cbi(DDR_SWITCH,SW2);\
					sbi(PORT_SWITCH,SW2);\
					cbi(DDR_SWITCH,SW3);\
					sbi(PORT_SWITCH,SW3);\
					cbi(DDR_SWITCH,SW4);\
					sbi(PORT_SWITCH,SW4);\
					cbi(DDR_SWITCH,SW_TEST);\
					sbi(PORT_SWITCH,SW_TEST)
//
//	Motor Left and servo
//	Timer 1. OCR1B ==> PWM0. OCR1A ==> PWM1. Duty = 20ms
//
#define DDR_MOTOR DDRB
#define PORT_MOTOR PORTB
#define PIN_MOTOR PINB
#define DIR_L 4
#define DIR_R 3
#define BF_L 6
#define BF_R 5

//Motor Left Init
//OCR1B (PD4)
#define MOTOR_LEFT_INIT		sbi(DDR_MOTOR,DIR_L);\
							cbi(PORT_MOTOR,DIR_L);\
							sbi(DDR_MOTOR,BF_L);\
							sbi(PORT_MOTOR,BF_L);\
							sbi(DDRD,4)
#define MOTOR_RIGHT_INIT	sbi(DDR_MOTOR,DIR_R);\
							cbi(PORT_MOTOR,DIR_R);\
							sbi(DDR_MOTOR,BF_R);\
							sbi(PORT_MOTOR,BF_R);\
							sbi(DDRD,7)
//Servo OCR1A, PD5
#define SERVO_INIT			sbi(DDRD,5)
//
//	Slave Enable
//
#define SLAVE_ENABLE_INIT	sbi(DDRC,4)
#define SLAVE_ON			sbi(PORTC,4)
#define SLAVE_OFF			cbi(PORTC,4)
// [Vo Huu Tai 9/8/2015 ]  Add, in running, USB must disable because PIN RX <=> PIN D-
//Disable by PC7
#define USB_DISABLE			cbi(DDRC,7);\
							cbi(DDRC,7)
/*TaiVH1 -- Aug 11, 2015  brief: Add for PWM, Sensor*/
//#define SENSOR_INTIT				(DDRA = 0x00)	//Use ADC
#define START_BAR_BIT				7
#define PWM_PERIOD_LEFT_MAX			22500
#define PEM_PERIOD_RIGHT_MAX		0xFF
//F = 12Mhz 1600 - 2400 - 3200
//#define SERVO_CENTER				2350//2400 
#define SERVO_ANGLE_STEP			8
// [Vo Huu Tai 12/8/2015 ]  
#define INVALID_NUM			0xFFFF
#define TEST_NONE					0
#define TEST_SENSOR_START_BAR		1
#define TEST_MOTOR					2

// [Vo Huu Tai 19/8/2015 ]  Add ADC & epprom
#define ADC_VREF_TYPE (1<<REFS0)
#define ADC_INIT	DDRA = 0x00;\
					ADCSRA = (1<<ADEN) | (1<<ADPS2) | (ADPS1) | (1<<ADPS0);\
					ADMUX = ADC_VREF_TYPE
// Macro for epprom. Must include <avr/eeprom.h>
#define read_eeprom_word(address)			eeprom_read_word ((const uint16_t*)address)
#define write_eeprom_word(address,value)	eeprom_write_word ((uint16_t*)address,(uint16_t)value)
#define update_eeprom_word(address,value)	eeprom_update_word ((uint16_t*)address,(uint16_t)value)
// [Vo Huu Tai 19/8/2015 ]  End Add ADC
// [Vo Huu Tai 20/8/2015 ]  Add extern interrupt
//Interrupt 1, any logic change
#define INT1_INIT		MCUCR = (1 << ISC10);\
						GICR  = (1 << INT1)
						
#define USE_PID

// [Vo Huu Tai 20/8/2015 ]  End add extern interrupt
// [Vo Huu Tai 21/8/2015 ]  ADD define for case car
#define CHECK_FROM_LEFT				1
#define CHECK_FROM_RIGHT			2

#define CAR_IDLE					0
#define CAR_NORMAL_TRACE			10

#define CAR_CROSS_LINE				20
#define CAR_TURN_LEFT				30
#define CAR_TURN_RIGHT				40

#define CAR_RIGHT_LINE				51
#define CAR_LEFT_LINE				61
// [Vo Huu Tai 23/8/2015 ]  ADD led 7 seg
#define _LED_DDR	DDRD
#define _LED_PORT	PORTD
#define SCK			0
#define DS			1
#define LATCH		2
#define LED7_INIT		sbi(_LED_DDR,SCK);\
						sbi(_LED_DDR,DS);\
						sbi(_LED_DDR,LATCH);
#endif /* COMMON_H_ */
