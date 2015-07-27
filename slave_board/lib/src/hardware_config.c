/*
 * hardware_config.c
 *
 * Created: 7/25/2015 9:47:23 PM
 *  Author: Vo Huu Tai
 */ 
#include "../inc/hardware_config.h"
#include "../inc/debug.h"
#include "../mpu6050/mpu6050.h"

void vInitProgram(){
	//disable local interrupt
	cli();
	
	//IO initialize
	BEEP_INIT;
	LED7_INIT;
	
	//Timer and PWM
	vInitTimer0();
	
	//serial
	v_init_debug_via_uart();
	
	//Mpu_6050
	vMPU6050Init();
	_delay_ms(100);
	
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