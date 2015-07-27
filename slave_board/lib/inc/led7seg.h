/*
 * led7seg.h
 *
 * Created: 7/25/2015 9:56:28 PM
 *  Author: Vo Huu Tai
 */ 


#ifndef LED7SEG_H_
#define LED7SEG_H_
#include "../inc/common.h"
/*
	@brief:		Call this function on timer interrupt
	@param:		none
	@return:	none
*/
extern void vScanLed();
/*
	@brief:		Use this function for out data to led 7
	@param:		Data to out
	@return:	none 
*/
extern void vOutLed7Seg(const unsigned int uiDataOut);


#endif /* LED7SEG_H_ */