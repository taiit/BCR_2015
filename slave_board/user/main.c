/*
 * slave_board.c
 *
 * Created: 7/25/2015 9:36:24 PM
 *  Author: Vo Huu Tai
 */ 

#include <stdlib.h>

#include "../lib/inc/hardware_config.h"
#include "../lib/inc/led7seg.h"
#include "../lib/inc/debug.h"
#include "../lib/inc/system_clock.h"
//#include "test_kalman.h"


int main(void)
{
	uint8_t *ucPrtData = (uint8_t *)calloc(DEBUG_BUFF_SIZE,sizeof(uint8_t));
	
	vInitProgram();		
	vOutLed7Seg(0000);
    while(1)
    {
		bDebugProcess();
		if(bMsgIsOK()){
			switch (ucGetCMDInfo())
			{
				case CMD_UPDATE_LED_7SEG:
					vSetCMDInfo(CMD_NONE);
					ucGetData(ucPrtData);
					vOutLed7Seg(ucPrtData[0] * 100 + ucPrtData[1]);
				break;
				case CMD_BEEP:
					vSetCMDInfo(CMD_NONE);
					ucGetData(ucPrtData);
					vBeepSlaver(ucPrtData[0] * 100 + ucPrtData[1]);
				break;
				case CMD_SENSOR:	//send kalAngleX to master board
					break;
				default:
				//vPutIntNum(ucGetCMDInfo(),DEC_TYPE);
				break;
			}//end witch();
		}//end if bMsgIsOK
    }
}
