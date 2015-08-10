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
			break;
		case 20:
		case 30:
		case 40:
		case 50:
		case 60:
		case 70:
		case 80:
		case 90:
			vLedFlash();
			uiTimer0Cnt = 0;
		break;
	}
}
int main(void)
{
	uint8_t *ucBuff = (uint8_t*)calloc(DEBUG_BUFF_SIZE,sizeof(uint8_t));
	int iSensorData;
	vInitProgram();	
	
	_delay_ms(1000);//waiting for slaver ready, remove it ...
	/**
	       vOutLed7(1234);		// Xuat so 1234		   
		   vBeep(100);			// Phat ra tieng beeep 100 ms
	*/	
    vOutLed7(1234);
	vBeep(100);	
	
	while(1)
    {       		
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
			
		}
    }
}
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 68;	//~1ms
	//Add more cnt here..
	vAssassin();
}