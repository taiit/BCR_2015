/*
 * unit_test.c
 *
 * Created: 7/20/2015 8:51:05 PM
 *  Author: Vo Huu Tai
 */ 
#include "uint_test.h"
#include "../../lib/inc/common.h"
#include "../../lib/inc/hardware_feature.h"
#include "../../lib/inc/LedBug.h"

void vLedDebugTest(){
	//test led debug. 
	// total: 40s
	for(int i = 0; i < 10; i++){
		LED_BUG_BLINK;
		_delay_ms(500);
	}	
	vLedCtrl(LED_FAST_BLINK);
	_delay_ms(10000);
	vLedCtrl(LED_SLOW_BLINK);
	_delay_ms(10000);
	vLedCtrl(LED_STARTUP_COMPELETE);
	_delay_ms(15000);
		
}

void vKeyTest(){
	bKeyIsPress(KEY1);
	bKeyIsPress(KEY2);
	bKeyIsPress(KEY3);
}

void vSwitchTest(){
	if(ucGetSwitch() == 1)LED_BUG_ON;
	else LED_BUG_OFF;
	
	//if(ucGetSwitch() == 2)LED_BUG_ON;
	//else LED_BUG_OFF;
	//
	//if(ucGetSwitch() == 4)LED_BUG_ON;
	//else LED_BUG_OFF;
	//
	//if(ucGetSwitch() == 8)LED_BUG_ON;
	//else LED_BUG_OFF;
}
void vSlaveTest(){
	SLAVE_ON;
	_delay_ms(1000);
	SLAVE_OFF;
	_delay_ms(1000);
}