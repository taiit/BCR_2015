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

/*TaiVH1 -- Aug 11, 2015  brief: Add for control motor and servo*/
extern void vMotor(int iSpeedLeft, int iSpeedRight);
extern void vSetBF(bool bBFMotorLeft,bool bBMotorRight);
extern void vServo(int iAngle);
extern uint8_t ucGetSensorData();
extern int iGetSensorPosition();
bool bStartBarIsStart();
int iGetInclined();
bool isTester();
/*TaiVH1 -- Aug 11, 2015  brief: End add for motor and servo*/
#endif /* HARDWARE_FEATURE_H_ */
