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
extern void vMotor(int iSpeedLeft, int iSpeedRight,bool bUsePID);
extern void vSetBF(bool bBFMotorLeft,bool bBMotorRight);
extern void vServo(int iAngle);
extern void vLoad2PServoCenter();
extern void vSetServoCenter(uint16_t uiData);
extern uint16_t uiGetServoCenter();
int iGetInclined();
bool isTester();
/*TaiVH1 -- Aug 11, 2015  brief: End add for motor and servo*/
// [Vo Huu Tai 19/8/2015 ]  add adc and epprom
uint16_t uiReadADC(unsigned char ucADCChanenel);
void vLearnColor();
uint8_t ucGetRawSensor();
extern uint8_t ucGetSensorData(uint8_t mask);
extern int iGetSensorPosition();
extern bool bStartBarClose();
extern uint16_t uiGetADCAverage(uint8_t ucIndex);
void vLoadE2PSensor();
// [Vo Huu Tai 19/8/2015 ]  end add adc and epprom
// [Vo Huu Tai 20/8/2015 ]  Add interrupt, PID
extern void vIncPulse();
extern void vCalPID();
// [Vo Huu Tai 20/8/2015 ]  End add interrupt, PID
// [Vo Huu Tai 23/8/2015 ]  Add module led 7 seg
extern void scanLed7();
extern void vOutLed7(unsigned int uiData);
extern void vOutLed1(uint8_t ucData);
extern void vBeep(unsigned int uiBeepTime);
//extern int iGetInlined();
// [Vo Huu Tai 23/8/2015 ]  end add
#endif /* HARDWARE_FEATURE_H_ */
