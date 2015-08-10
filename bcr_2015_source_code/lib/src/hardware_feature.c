/*
 * hardware_feature.c
 *
 * Created: 7/20/2015 8:48:30 PM
 *  Author: Vo Huu Tai
 */ 
#include "../inc/hardware_feature.h"
#include "../inc/debug.h"

bool bKeyIsPress(uint8_t ucKeyID){
	uint8_t mask = (1<<ucKeyID);
	//key is pressed when pin = 0;
	if((PIN_KEY & mask) != mask){
		_delay_ms(10);
		if((PIN_KEY & mask) != mask){
			//LED_BUG_ON;
			_delay_ms(10);
			while((PIN_KEY&mask)!=mask);//waiting for key is released
			//LED_BUG_OFF;
			vBeep(30);
			return true;
		}
	}
	return false;
}

uint8_t ucGetSwitch(){
	uint8_t mask = (1<<SW1) | (1<<SW2) | (1<<SW3) | (1<<SW4);
	uint8_t binData = (~PIN_SWITCH) & mask; 	
	// Must swap switch bit
	//binData = 0000 1101 => 0000 1011 		
	return binData;
}

