/*
 * hardware_feature.h
 *
 * Created: 7/20/2015 8:48:52 PM
 *  Author: Vo Huu Tai
 */ 


#ifndef HARDWARE_FEATURE_H_
#define HARDWARE_FEATURE_H_
#include "../inc/common.h"

/*
	@brief:		check key is pressed
	@param:		keyID: KEY1, KEY2, KEY3
	@return:	true if key is pressed 
*/
extern bool bKeyIsPress(uint8_t ucKeyID);
/*
	@brief:		get data from dip switch
	@param:		none
	@return:	data form 0x00 to 0xFF
*/

extern uint8_t ucGetSwitch();

#endif /* HARDWARE_FEATURE_H_ */