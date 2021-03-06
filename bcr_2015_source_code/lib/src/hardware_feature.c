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
			LED_BUG_ON;
			_delay_ms(10);
			while((PIN_KEY&mask)!=mask);//waiting for key is released
			LED_BUG_OFF;
			vBeep(30);
			return true;
		}
	}
	return false;
}

uint8_t ucGetSwitch(){
	uint8_t mask = (1 << SW1) | (1 << SW2) | (1 << SW3) | (1 << SW4);
	uint8_t binData = (~PIN_SWITCH) & mask; 	
	// Must swap switch bit
	//binData = 0000 1101 => 0000 1011 		
	return binData;
}

/*TaiVH1 -- Aug 11, 2015  brief: Add for control motor and servo*/
int ucExSpeedLeft = 0, ucExSpeedRight = 0;
volatile bool bMotorUsePID = false;
int SERVO_CENTER	=			2350;

void vMotor(int iSpeedLeft, int iSpeedRight,bool bUsePID){
	#if 0
	switch((ucGetSwitch()&0x07)){//0000 0xxx
		case 0x01:
			if(iSpeedLeft>=0)iSpeedLeft -=5;
			//else iSpeedLeft += 5;
		
			if(iSpeedRight>=0)iSpeedRight -= 5;
			//else iSpeedRight -= 5;
		break;
		case 0x02:
			if(iSpeedLeft>=0)iSpeedLeft -= 8;
			//else iSpeedLeft += 5;
			
			if(iSpeedRight>=0)iSpeedRight -= 8;
			//else iSpeedRight -= 5;
			break;
		case 0x03:
			if(iSpeedLeft>=0)iSpeedLeft -= 10;
			//else iSpeedLeft += 5;
			
			if(iSpeedRight>=0)iSpeedRight -= 10;
			//else iSpeedRight -= 5;
			break;
	}
	#endif
	bMotorUsePID = bUsePID;
	if(bUsePID){
		ucExSpeedLeft = iSpeedLeft;
		ucExSpeedRight = iSpeedRight;	
	}
	else{
		//uint8_t ucSwData = ucGetSwitch() + 5;//ucSwData = [0..20]
		int iSpeedOfset ;//= (PWM_PERIOD_LEFT_MAX-1)*((float)(ucSwData/20));
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
}
void vSetBF(bool bBFMotorLeft,bool bBFMotorRight){
	//BF motor left
	if(bBFMotorLeft == true){
		cbi(PORT_MOTOR,BF_L);
	}else{
		sbi(PORT_MOTOR,BF_L);
	}
	//BF motor right
	if(bBFMotorRight == true){
		cbi(PORT_MOTOR,BF_R);
	}else{
		sbi(PORT_MOTOR,BF_R);
	}
}
void vServo(int iAngle){
	OCR1A = SERVO_CENTER + iAngle*SERVO_ANGLE_STEP;
}
// [Vo Huu Tai 19/8/2015 ]  ADD 
		/// .. . .
// [Vo Huu Tai 19/8/2015 ]  end ADD

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

// [Vo Huu Tai 19/8/2015 ]  Add for ADC and epprom
uint16_t EEMEM  uiMyEppDataArr[9];
uint16_t uiAdAverage[8],uiAdMin[8],uiAdMax[8];

PUBLIC uint16_t uiReadADC(unsigned char chanenel)
{
	ADMUX = chanenel|ADC_VREF_TYPE;
	ADCSRA |= (1<<ADSC);
	loop_until_bit_is_set(ADCSRA,ADIF);
	return ADCW;
}
PUBLIC void vLearnColor()
{
	uint16_t temp = 0;
	while(1)
	{  
		vOutLed7(99);
		//ADC0	ADC1	ADC2				ADC6	ADC7
		// xx	 xx		 xx	 x-x	xx	xx	 xx      --
		for(uint8_t i = 0; i < 7; i++)
		{
			temp = uiReadADC(i);
			_delay_us(50);
			if(uiAdMax[i] == 0) uiAdMax[i] = temp;
			else if(temp > uiAdMax[i]) uiAdMax[i] = temp;
			if(uiAdMin[i] == 0) uiAdMin[i] = temp;
			else if(temp < uiAdMin[i]){
				uiAdMin[i] = temp;
			}
		}
		if(bKeyIsPress(KEY3)) break;//Exit
	}//end while(1)
	vOutLed7(66);
	vBeep(100);	_delay_ms(100);	vBeep(100);
	uiAdAverage[7] = 111;//Note, start bar is bit 0. if adc[7] < adc_average[7] it mean start bar is open
	write_eeprom_word(&uiMyEppDataArr[7] , uiAdAverage[7]);
	for (uint8_t i = 0; i < 7; i++)
	{
		//uiAdAverage[i] = (uiAdMin[i] + uiAdMax[i])/2;
	
		uiAdAverage[i] = 500;//= uiAdAverage[i]*((float)1.25);
		write_eeprom_word(&uiMyEppDataArr[i] , uiAdAverage[i]);
	}
}
PUBLIC uint8_t ucGetRawSensor()
{
	uint8_t result = 0;
	uint16_t temp = 0;
	//ADC0	ADC1	ADC2				ADC6	ADC7
	// xx	 xx		 xx	 x-x	xx	xx	 xx      --
	for(int i = 0; i < 7;i++)
	{
		temp = uiReadADC(i);
		//swap bit here
		if(temp <= uiAdAverage[i]){
			sbi(result,6-i);
		}
		else{
			cbi(result,6-i);	
		}		
	}
	cbi(result,7);
	//data_led = result;
	vOutLed1(result);
	return result;
}
PUBLIC uint8_t ucGetSensorData(uint8_t mask){

	return (ucGetRawSensor() & mask);
}
int iGetSensorPosition(){
	return 0;
}
bool bStartBarClose(){
	uint16_t ucDataADC = uiReadADC(7);
	if(ucDataADC < 100) return true;//adc_average[7]
	return false;
}
void vLoadE2PSensor(){
	for(int i = 0;i < 8;i++){
		uiAdAverage[i] = read_eeprom_word(&uiMyEppDataArr[i]);
	}
	
}
uint16_t uiGetADCAverage(uint8_t ucIndex){
	return(uiAdAverage[ucIndex]);
}
void vLoad2PServoCenter(){
	SERVO_CENTER = read_eeprom_word(&uiMyEppDataArr[8]);
}
void vSetServoCenter(uint16_t uiData){
	SERVO_CENTER = uiData;
	write_eeprom_word(&uiMyEppDataArr[8],SERVO_CENTER);
}
uint16_t uiGetServoCenter(){
	return SERVO_CENTER;
}
// [Vo Huu Tai 19/8/2015 ]  End add ADC and epprom
// [Vo Huu Tai 20/8/2015 ]  Add interrupt

volatile uint16_t uiPulse;
int pre_ErrLeft = 0,pre_ErrRight = 0,uiLastPulse = 0;
float outputMotorLeft = 0;
float outputMotorRight = 0;

void vIncPulse(){
	uiPulse++;
}
PRIVATE int uiGetDeltaPulse(){
		
	int uiDeltaPulse = uiPulse - uiLastPulse;
	uiLastPulse = uiPulse;	
	
	return (uiDeltaPulse);
}
PRIVATE float iGetErr(int speed,int deltaPluse){
	float temp = 40*speed*0.01;
	return(temp - deltaPluse);
}
void vCalPID(){
	//call over 10ms
	float Kp,Kd;
	float P,D,err,deltaPulse;		
	
	deltaPulse = uiGetDeltaPulse();//alway get delta pluse for reset review pluse.
	
	if(!bMotorUsePID)return;	
	
	//motor left
	Kp = 29.9;
	Kd = 12.1;
	
	err = iGetErr(ucExSpeedLeft,deltaPulse);
	P = Kp * err;
	D = Kd*(err - pre_ErrLeft)*50; //50 is inv_Sampling_time
	outputMotorLeft += P + D;

	if(outputMotorLeft > (PWM_PERIOD_LEFT_MAX - 1000))outputMotorLeft = PWM_PERIOD_LEFT_MAX - 1000;
	if (outputMotorLeft < 0)outputMotorLeft = 0;
//
	OCR1B = outputMotorLeft;
	pre_ErrLeft = err;
//motor right
	Kp = 0.75;
	Kd = 0.12;
	
	err = iGetErr(ucExSpeedRight,deltaPulse);
	P = Kp*err;
	D = Kd*(err - pre_ErrRight)*50;//50 is inv_Sampling_time
	outputMotorRight += P + D;
	
	if(outputMotorRight > (PEM_PERIOD_RIGHT_MAX - 10))outputMotorRight = PEM_PERIOD_RIGHT_MAX - 10;
	if(outputMotorRight < 0)outputMotorRight = 0;
	
	OCR2 = outputMotorRight;
	pre_ErrRight = err;
	
	//Dir
	if(ucExSpeedLeft > 0)
	sbi(PORT_MOTOR,DIR_L);
	else cbi(PORT_MOTOR,DIR_R);
	
	if(ucExSpeedRight > 0)
	cbi(PORT_MOTOR,DIR_R);
	else 
	sbi(PORT_MOTOR,DIR_R);
}
// [Vo Huu Tai 20/8/2015 ]  End add interrupt

// [Vo Huu Tai 23/8/2015 ]  Add module led 7 seg
uint8_t led7seg_index = 0;
unsigned char ucDataLed1 = 0;
int uiDataLed7 = 0;
void scanLed7()
{
	if(uiDataLed7 < 0)uiDataLed7 = -(uiDataLed7);
	uint8_t i, temp, mask_led = 0x01,mask_seg = 0x80;
	uint8_t donvi = (uiDataLed7 % 10 << 4)|0x08;
	uint8_t chuc = ((uiDataLed7%100) / 10 << 4)|0x04;
	uint8_t tram = ((uiDataLed7%1000) / 100 << 4)|0x02;
	uint8_t ngan = ((uiDataLed7  / 1000) << 4)|0x01;

	sbi(_LED_PORT,LATCH);
	for (i=0; i< 16; i++)
	{
		cbi(_LED_PORT,SCK);
		//xuat ra 8 led don
		if(i > 7)
		{
			temp = ucDataLed1 & mask_led;
			if(temp == 0)sbi(_LED_PORT,DS);
			else cbi(_LED_PORT,DS);
			mask_led = mask_led << 1;
		}
		else //xuat ra module led 7 doan
		{
			switch(led7seg_index)
			{
				case 0:
				{
					temp = ngan & mask_seg;
					if(temp == 0)cbi(_LED_PORT,DS);
					else sbi(_LED_PORT,DS);
					mask_seg >>= 1;
					break;
				}
				case 1:
				{
					temp = tram & mask_seg;
					if(temp == 0)cbi(_LED_PORT,DS);
					else sbi(_LED_PORT,DS);
					mask_seg >>= 1;
					break;
				}
				case 2:
				{
					temp = chuc & mask_seg;
					if(temp == 0)cbi(_LED_PORT,DS);
					else sbi(_LED_PORT,DS);
					mask_seg >>= 1;
					break;
				}
				case 3:
				{
					temp = donvi & mask_seg;
					if(temp == 0)cbi(_LED_PORT,DS);
					else sbi(_LED_PORT,DS);
					mask_seg >>= 1;
					break;
				}
				default:
				break;
			}
		}
		sbi(_LED_PORT,SCK);
	}
	//end for
	cbi(_LED_PORT,LATCH); //out 595
	led7seg_index++;
	if(led7seg_index == 4)led7seg_index = 0;
}
PUBLIC void vOutLed7(unsigned int uiData){
	uiDataLed7 = uiData;
}
PUBLIC void vOutLed1(uint8_t ucData){
	ucDataLed1 = ucData;
}
PUBLIC void vBeep(unsigned int uiBeepTime){
	//No beep, do nothing
}