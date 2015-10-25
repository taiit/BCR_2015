/*
 * hardware_config.c
 *
 * Created: 7/20/2015 8:21:39 PM
 *  Author: Vo Huu Tai
 */ 
#include "../inc/hardware_config.h"
//#include "../inc/debug.h"

void vInitProgram(){
	//disable local interrupt
	cli();
	
	//IO initialize
	LED_BUB_INIT;		//Led debug is output
	KEY_INIT;			//Key is input
	SWITCH_INIT;		//Switch is input
	MOTOR_LEFT_INIT;
	MOTOR_RIGHT_INIT;
	SERVO_INIT;
	SLAVE_ENABLE_INIT;
	SLAVE_ON;
	USB_DISABLE;
	//SENSOR_INTIT; //Use ADC
	ADC_INIT;
	INT1_INIT;
	LED7_INIT;
	
	//Timer and PWM
	vInitTimer0();
	vInitTimer1();
	vInitTimer2();	
	
	//SERIAL
	//v_init_debug_via_uart();
	//enable local interrupt
	sei();
}
void vInitTimer0(){
	/**
	       Timer 0.
				Normal mode.
				F = 12Mhz
				T = 5.33333us
				=> prescaler = 64
	*/
	TCCR0 = (1 << CS01) | (1<<CS00);
	TIMSK |= (1 << TOIE0);
}
void vInitTimer1(){
	/**
	       Timer 1.
				PWM mode 14: WGM1[3:0] = 1110 
				Non-inverting: COM1[1:0] = 10
				F = 12Mhz
				T = 15ms
				=> prescaler = 8.
				ICR1 = 15ms / ((1/12Mhz)*8) = 22500	
	*/
	TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = PWM_PERIOD_LEFT_MAX;
	//OCR1A = uiGetServoCenter(); //Servo, 1ms <-> 1500 and 2ms <-> 3000
	OCR1B = 0;//Motor Left
}
void vInitTimer2(){
	/**
	       Timer 2.
				PWM mode 3: WGM2[1:0] = 11
				Non-inverting: COM2[1:0] = 10
				F = 12Mhz
				T = 5.5ms
				=> prescaler = 256.
	*/
	TCCR2 = (1 << WGM21) | (1 << WGM20) | (1 << COM21) | (1 << CS22) | (1 << CS21);
	//OCR2 = PWM_ICR2_MAX;
}
