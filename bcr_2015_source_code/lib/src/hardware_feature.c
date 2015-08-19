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
	uint8_t ucSwData = ucGetSwitch() + 5;//ucSwData = [0..20]
	int iSpeedOfset = (PWM_PERIOD_LEFT_MAX-1)*((float)(ucSwData/20));
	iSpeedOfset = PWM_PERIOD_LEFT_MAX - 1; //Fix me
	//Speed motor left
	if(iSpeedLeft >= 0){
		sbi(PORT_MOTOR,DIR_L);
		OCR1B = iSpeedOfset * ((float)(iSpeedLeft * 0.01));
	}
	else{
		cbi(PORT_MOTOR,DIR_L);
		OCR1B = iSpeedOfset * (float)((-iSpeedLeft) * 0.01);
	}
	//Speed motor right
	iSpeedOfset = PEM_PERIOD_RIGHT_MAX - 1;
	if(iSpeedRight >= 0){
		cbi(PORT_MOTOR,DIR_R);
		OCR2  = iSpeedOfset * ((float)(iSpeedRight * 0.01));
	}
	else{
		sbi(PORT_MOTOR,DIR_R);
		OCR2  = iSpeedOfset * ((float)((-iSpeedRight) * 0.01));
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

uint8_t ucGetSensorData(){

	return 0;
}

int iGetSensorPosition(){
	return 0;
}
bool bStartBarIsStart(){
	if(bit_is_clear(PINA,START_BAR_BIT))return true;
	return false;
}
int iGetInclined(){
	return 0;
}
/*TaiVH1 -- Aug 11, 2015  brief: End add for motor and servo*/

// [Vo Huu Tai 12/8/2015 ]  Add for inclined and tester

bool isTester(){
	if((PIN_SWITCH&(1<<SW_TEST)) == (1<<SW_TEST) )return false;
	return true;
}
// [Vo Huu Tai 12/8/2015 ]  end add for inclined and tester