/*
 * system_clock.h
 *
 * Created: 4/4/2015 4:11:41 PM
 *  Author: Vo Huu Tai
 */ 


#ifndef SYSTEM_CLOCK_H_
#define SYSTEM_CLOCK_H_

#include "../inc/led7seg.h"

struct tick
{	
	 unsigned long ulmsec;	
	 
}volatile S_SYSTEM_CLOCK_TICK;

volatile uint16_t uiTimer0Cnt = 0;

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 68; //~1ms
	
	if(S_SYSTEM_CLOCK_TICK.ulmsec++ > 36000000)//10 hour
	{
		S_SYSTEM_CLOCK_TICK.ulmsec = 0;		
	}
	uiTimer0Cnt++;
	if(uiTimer0Cnt == 5){//5ms
		uiTimer0Cnt = 0;
		vScanLed();
	}
	
}

unsigned long sys_clk_get_msec()
{	
	return (S_SYSTEM_CLOCK_TICK.ulmsec);
}


#endif /* SYSTEM_CLOCK_H_ */