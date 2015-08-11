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

/*TaiVH1 -- Aug 11, 2015  brief: Add for control motor and servo*/

void vMotor(int iSpeedLeft, int iSpeedRight){
	//Speed motor left
	if(iSpeedLeft >= 0){
		sbi(PORT_MOTOR,DIR_L);
		OCR1B = iSpeedLeft * PWM_ICR1_MAX * 0.01;
	}
	else{
		cbi(PORT_MOTOR,DIR_L);
		OCR1B = (-iSpeedLeft) * PWM_ICR1_MAX * 0.01;
	}
	//Speed motor right
	if(iSpeedRight >= 0){
		sbi(PORT_MOTOR,DIR_R);
		OCR2 = iSpeedRight * PWM_ICR2_MAX * 0.01;
	}
	else{
		cbi(PORT_MOTOR,DIR_R);
		OCR2 = (-iSpeedRight) * PWM_ICR2_MAX * 0.01;
	}
}
void vSetBF(bool bBFMotorLeft,bool bBFMotorRight){
	//BF motor left
	if(bBFMotorLeft == true){
		sbi(PORT_MOTOR,BF_L);
	}else{
		cbi(PORT_MOTOR,BF_L);
	}
	//BF motor right
	if(bBFMotorRight == true){
		sbi(PORT_MOTOR,BF_R);
	}else{
		cbi(PORT_MOTOR,BF_R);
	}
}
void vServo(int iAngle){
	OCR1A = SERVO_CENTER + iAngle*SERVO_ANGLE_STEP;
}

uint8 ucGetSensorData(){

	uint8_t ucSensorRawData = PORTA;
	/*Check start bar bit*/
	#if	(START_BAR_BIT == 7)
		ucSensorRawData &= 0x7f;
	#elif (START_BAR_BIT == 0)
		ucucSensorRawData >>= 1;
	#else
	#error SENSOR_BIT INCORRECT
	#endif
	return ucSensorRawData;
}

int iGetSensorPosition(){
	/*
	 * 			S1	 S2	 S3		S4		S5	S6	S7
	 *
	 * 			-30	-20	-10		0		10	20	30
	 * 	index    6	 5	 4		3		2	1	0
	 *
	 * 				S1*(-10) + S2*(-20) + ...
	 * 	position = ----------------------------
	 * 				S1    +    S2       + ...
	 */
	int iPosition = 0;
	uint8_t ucSensorRawData = ucGetSensorData();
	uint8_t ucIndexCnt = 0,ucSumWhiteBit = 0;

	for(; ucIndexCnt < 7; ucIndexCnt++){
		if(bit_is_clear(ucSensorRawData,ucIndexCnt)){ //White line => 0
			ucSumWhiteBit++;
			switch(ucIndexCnt){
				case 0:
					iPosition += 30;
					break;
				case 1:
					iPosition += 20;
					break;
				case 2:
					iPosition += 10;
					break;
				case 3:
					iPosition += 0;
					break;
				case 4:
					iPosition += -10;
					break;
				case 5:
					iPosition += -20;
					break;
				case 6:
					iPosition += -30;
					break;
			}
		}else{											//Black line => 1
			//do nothing
		}
	}
	if(ucSumWhiteBit != 0){
		iPosition /= ucSumWhiteBit;
	}
	return iPosition;
}
bool bStartBarIsStart(){
	if(bit_is_clear(PORTA,SENSOR_BIT))return true;
	return false;
}
/*TaiVH1 -- Aug 11, 2015  brief: End add for motor and servo*/
