/*
 * main.c
 *
 * Created: 7/20/2015 8:07:28 PM
 *  Author: Vo Huu Tai
 */ 


#include "../lib/inc/hardware_config.h"
#include "../lib/inc/hardware_feature.h"
#include "unit_test/uint_test.h"
#include "../lib/inc/LedBug.h"
#include "../lib/inc/debug.h"

volatile uint16_t uiTimer0Cnt = 0;

void vAssassin(){
	// [Vo Huu Tai 10/8/2015 ]  over 1ms
	if(++uiTimer0Cnt >= 10)uiTimer0Cnt = 0;
	switch(uiTimer0Cnt){//I wish it is called very 10ms		
		case 0:
			vInclinedPoll();
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			vLedFlash();
		break;
	}
}

int main(void)
{
	
	int iTempData = 0,iLeft = 0,iRight = 0;
	uint8_t ucTestPattern = TEST_NONE;
	vInitProgram();		
	_delay_ms(1000);//waiting for slaver ready, remove it ...
	/**
	       vOutLed7(1234);		// Xuat so 1234		   
		   vBeep(100);			// Phat ra tieng beeep 100 ms
		   vMotor(10,-10);		//	Speed Left = 10, speed right = -10
		   
	*/	
    vOutLed7(1234);
	vBeep(100);	
	vLedCtrl(LED_STARTUP_COMPELETE);
	while(1)
    {       		
		bDebugProcess();
		if(isTester()){//check switch tester function
		//Test go here
			switch(ucTestPattern){
				case TEST_SENSOR_START_BAR:
					if(bKeyIsPress(KEY3)) ucTestPattern = TEST_NONE;//exit from test
					if(bKeyIsPress(KEY1)){
						if(bStartBarIsStart())vLedCtrl(LED_START_BAR_OK);//Test start bar
					}
					//alway test sensor Test sensor
					//iTempData = iGetSensorPosition();
					//if(iTempData >= 0){
					//	vOutLed7(iTempData);
					//	}else{
					//	vOutLed7(-iTempData + 1000);
					//}
					break;
				case TEST_MOTOR:
					if(bKeyIsPress(KEY3)){
						 ucTestPattern = TEST_NONE;//exit from test
						 vMotor(0,0);
					}
					if(bKeyIsPress(KEY1)){
						iLeft++;
						iRight++;
					}
					if(bKeyIsPress(KEY2)){
						iLeft--;
						iRight--;
					}
					vMotor(iLeft,iRight);
					break;
				case TEST_NONE://DEFAULT ALWAYS TEST INCLINED
					
					if(bKeyIsPress(KEY1)){//KEY1 - SENSOR AND START BAR
						vOutLed7(1);
						ucTestPattern = TEST_SENSOR_START_BAR;
					}
					if(bKeyIsPress(KEY2)){//KEY2 - MOTOR
						vOutLed7(2);
						iLeft = iRight = 10;
						ucTestPattern = TEST_MOTOR;
					}
					
					iTempData = iGetInclined();
					if(iTempData != INVALID_NUM){
						if(iTempData > 0){
							vOutLed7(iTempData);
						}
						else{
							vOutLed7(-iTempData + 1000);
						}
					}
					break;
				default:
					break;
			}//End switch test case			
		}//End tester
//////////////////////////////////////////////////////////////////////////
		else{// Normal run
			
		}//end Normal run		
//////////////////////////////////////////////////////////////////////////
    }//End while 1
}
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 68;	//~1ms
	//Add more cnt here..
	vAssassin();
}
