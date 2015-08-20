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
	uiTimer0Cnt++;
	switch(uiTimer0Cnt){//I wish it is called very 10ms		
		case 10:
			iGetInlined();
			vLedFlash();
			uiTimer0Cnt = 0;
			break;
	}
	// [Vo Huu Tai 14/8/2015 ]  BUGGY here :(((
}
int main(void)
{
	uint8_t *ucBuff = (uint8_t*)calloc(DEBUG_BUFF_SIZE,sizeof(uint8_t));
	int iSensorData = 0;
	vInitProgram();	
	
	_delay_ms(1000);//waiting for slaver ready, remove it ...
	/**
	       vOutLed7(1234);		// Xuat so 1234		   
		   vBeep(100);			// Phat ra tieng beeep 100 ms
	*/	
    vOutLed7(1234);
	vBeep(100);	
	vMotor(-30,-30);
	vServo(0);
	while(1)
    {       	
		if(isTester()){//Get switch tester
			vOutLed7(ucGetSwitch());
			bDebugProcess();
			if(bMsgIsOK()){
				switch (ucGetCMDInfo())
				{
					case CMD_SENSOR:
					vSetCMDInfo(CMD_NONE);
					ucGetData(ucBuff);
					if (ucBuff[0] == 1){//negative
						iSensorData = -ucBuff[1];
						vOutLed7((-iSensorData + 1000));
					}
					if(ucBuff[0] == 0){
						iSensorData = ucBuff[1];
						vOutLed7(iSensorData);
					}
					
					break;
					default: break;
				}
			}
			
			if(bKeyIsPress(KEY1)){
				vLedCtrl(LED_STARTUP_COMPELETE);
			}
			if(bStartBarIsStart()){
				vLedCtrl(LED_STARTUP_COMPELETE); //Fix me
			}
		}//end tester
		else{//normal running
			if(bKeyIsPress(KEY2)){//Key 2 learn color, and run
				vLearnColor();
				iSensorData = 0;
				while(1){
					if (bKeyIsPress(KEY1))
					{
						//xx xx xx  xx  xx xx xx    --
						iSensorData ++;
						if(iSensorData == 8){
							iSensorData = 0;
							vLedCtrl(LED_START_BAR_OK);
						}						
					}
					//vOutLed7(read_adc(iSensorData));
					vOutLed7(ucGetSensorData(0xff));
					_delay_ms(100);
				}//end while 1
			}//end Key 2 learn color, and run
			if(bKeyIsPress(KEY1)){//Load epprom and run
				vLoadE2P();
				while(1){
					vOutLed7(ucGetSensorData(0xff));
					_delay_ms(100);
				}
			}// end Load epprom and run
		} //end normal running
    }//end while(1)
}
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 68;	//~1ms
	//Add more cnt here..
	vAssassin();
}
