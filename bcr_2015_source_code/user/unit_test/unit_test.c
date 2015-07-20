/*
 * unit_test.c
 *
 * Created: 7/20/2015 8:51:05 PM
 *  Author: Vo Huu Tai
 */ 
#include "uint_test.h"
#include "../../lib/inc/common.h"
#include "../../lib/inc/hardware_feature.h"

void vLedDebugTest(){
	LED_BUG_BLINK;
	_delay_ms(1000);
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