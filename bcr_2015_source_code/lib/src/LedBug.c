/*
 * LedBug.c
 *
 * Created: 7/22/2015 8:16:42 PM
 *  Author: Vo Huu Tai
 */ 
#include "../inc/LedBug.h"

volatile uint8_t ucgLedState = LED_STATE_NONE;
volatile bool bgLedFlag = LED_FLAG_OFF;
volatile uint16_t uigLedPeriodic = NO_LIMIT;
volatile uint16_t uigLedInterval = NO_LIMIT;
uint8_t ucLedEvent = LED_NONE;
//
//internal function
//
/*
	@brief:		Decrease periodic counter
	@param:		none
	@return:	none 
*/

void vPeriodicDec(){	
	if(uigLedPeriodic == 0 ){
		uigLedPeriodic = NO_LIMIT;
	}else
	uigLedPeriodic--;	
}
/*
	@brief:		Decrease interval counter
	@param:		none
	@return:	none 
*/
void vIntervalDec(){
	
	if(uigLedInterval == 0){
		uigLedInterval = NO_LIMIT;
	}
	else
	uigLedInterval--;	
}
/*
	@brief:		parse control for led
	@param:		led event
	@return:	Led Periodic counter
*/
uint16_t ucPareLedEvent(uint8_t led_event){
	uint16_t uiRet = NO_LIMIT;
	switch(led_event){
		case LED_FAST_BLINK:			
			uigLedInterval = LED_50_MS;			
			break;
		case LED_SLOW_BLINK:			
			uigLedInterval = LED_100_MS;			
			break;
		case LED_STARTUP_COMPELETE:			
			uigLedInterval = LED_50_MS;	
			uiRet = LED_10_S;	
			break;		
		default:			
			break;
	}
	return uiRet;
}
//
void vLedFlash(){
	uint8_t ucStateTemp = ucgLedState;	
	
	switch(ucStateTemp){
		case LED_STATE_NONE:	//State NONE do nothing.
			return;
		case LED_STATE_BLINK:		
			vPeriodicDec();
			if(uigLedPeriodic == 0){
				LED_BUG_OFF; //turn off led
				bgLedFlag = LED_FLAG_OFF;//set flag is off
				ucStateTemp = LED_STATE_NONE;
			}
			else{
				vIntervalDec();
				if(uigLedInterval == 0){
					switch (bgLedFlag)
					{
						case LED_FLAG_ON:
							bgLedFlag = LED_FLAG_OFF;
							LED_BUG_OFF;
							break;
						case LED_FLAG_OFF:
							bgLedFlag = LED_FLAG_ON;
							LED_BUG_ON;
							break;
						default:							
							break;
					}					
					ucPareLedEvent(ucLedEvent);					
				}				
			}
			break;
		default:			
			break;		
	}		
	ucgLedState = ucStateTemp;		
}

extern void vLedCtrl(uint8_t E_EVENT){
	
	uigLedPeriodic = ucPareLedEvent(E_EVENT);
	ucgLedState = LED_STATE_BLINK;	
	ucLedEvent = E_EVENT;
	
}