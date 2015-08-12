/*
 * LedBug.h
 *
 * Created: 7/22/2015 8:16:26 PM
 *  Author: Vo Huu Tai
 */ 


#ifndef LEDBUG_H_
#define LEDBUG_H_
#include "../inc/common.h"
enum{
	LED_NONE,
	LED_FAST_BLINK,
	LED_SLOW_BLINK,
	LED_START_BAR_OK,
	LED_STARTUP_COMPELETE	
}LED_EVENT;

enum{
	LED_STATE_NONE,	
	LED_STATE_BLINK	
}LED_STATE;

#define LED_FLAG_ON		true
#define LED_FLAG_OFF	false

#define NO_LIMIT 0xFFFF

#define LED_50_MS		5		// 10ms * 5 = 50ms
#define LED_100_MS		10		// 10ms * 10 = 100ms
#define LED_500_MS		50		// 10ms * 50 = 500ms
#define LED_1_S			100		// 10ms * 100 = 1s
#define LED_10_S		1000	// 10ms * 1000 = 10s
/*
	@brief:		Scan Led Bug. Call after every 10ms
	@param:		none
	@return:	none 
*/
extern void vLedFlash();
/*
	@brief:		Control Led Bug
	@param:		Event for state of led
	@return:	none 
*/
extern void vLedCtrl(uint8_t EVENT);

#endif /* LEDBUG_H_ */