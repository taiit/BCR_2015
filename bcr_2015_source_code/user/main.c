/*
 * main.c
 *
 * Created: 7/20/2015 8:07:28 PM
 *  Author: Vo Huu Tai
 */ 


#include "../lib/inc/hardware_config.h"
#include "../lib/inc/hardware_feature.h"
#include "unit_test/uint_test.h"
#include "../lib/inc/LedBug.h"
#include "../lib/inc/debug.h"

int main(void)
{
	vInitProgram();	
	
	_delay_ms(1000);
	/**
	       vOutLed7(1234);		// Xuat so 1234		   
	
	*/	
    vOutLed7(1234);
	while(1)
    {
       
		
		
    }
}
volatile uint16_t uiTimer0Cnt = 0;
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 68;	//~1ms
	//Add more cnt here..
	
	
	// [Vo Huu Tai 9/8/2015 ]
	uiTimer0Cnt++;
	if(uiTimer0Cnt == 10){//10ms
		uiTimer0Cnt = 0;
		vLedFlash(); 
	}
	
}