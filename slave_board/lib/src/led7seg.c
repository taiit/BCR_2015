/*
 * led7seg.c
 *
 * Created: 7/25/2015 9:58:42 PM
 *  Author: Vo Huu Tai
 */ 
#include "../inc/led7seg.h"

uint8_t g_uc_led7seg_index;
uint16_t g_ui_data_led7seg = 0;

PUBLIC void vOutLed7Seg(const uint16_t uiDataOut){
	g_ui_data_led7seg = uiDataOut;	
}

PUBLIC void vBeepSlaver(unsigned int uiBeepTime){
	BEEP_ON;
	while(uiBeepTime--){
		_delay_ms(1);
	}
	BEEP_OFF;
}

void vScanLed(){
	
	uint8_t ucIndex, ucDataTemp,ucMaskLed7Seg = 0x80;
	uint8_t ucUnit = (g_ui_data_led7seg % 10)				|0x80;
	uint8_t ucTen = ((g_ui_data_led7seg % 100) / 10)		|0x40;
	uint8_t ucHundred = ((g_ui_data_led7seg % 1000) / 100)	|0x20;
	uint8_t ucThousand = ((g_ui_data_led7seg  / 1000))		|0x10;

	sbi(PORT_LED7,LATCH);
	for (ucIndex = 0; ucIndex < 8; ucIndex++)
	{
		cbi(PORT_LED7,SCK);		
		switch(g_uc_led7seg_index)
		{
			case 0:
			{
				ucDataTemp = ucThousand & ucMaskLed7Seg;
				if(ucDataTemp == 0)cbi(PORT_LED7,DATA);
				else sbi(PORT_LED7,DATA);
				ucMaskLed7Seg >>= 1;
				break;
			}
			case 1:
			{
				ucDataTemp = ucHundred & ucMaskLed7Seg;
				if(ucDataTemp == 0)cbi(PORT_LED7,DATA);
				else sbi(PORT_LED7,DATA);
				ucMaskLed7Seg >>= 1;
				break;
			}
			case 2:
			{
				ucDataTemp = ucTen & ucMaskLed7Seg;
				if(ucDataTemp == 0)cbi(PORT_LED7,DATA);
				else sbi(PORT_LED7,DATA);
				ucMaskLed7Seg >>= 1;
				break;
			}
			case 3:
			{
				ucDataTemp = ucUnit & ucMaskLed7Seg;
				if(ucDataTemp == 0)cbi(PORT_LED7,DATA);
				else sbi(PORT_LED7,DATA);
				ucMaskLed7Seg >>= 1;
				break;
			}
			default:
			break;
		}		
		sbi(PORT_LED7,SCK);
	}//end for
	cbi(PORT_LED7,LATCH); //out 595
	g_uc_led7seg_index++;
	if(g_uc_led7seg_index == 4)g_uc_led7seg_index = 0;
}