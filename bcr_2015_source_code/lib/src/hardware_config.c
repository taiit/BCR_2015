/*
 * hardware_config.c
 *
 * Created: 7/20/2015 8:21:39 PM
 *  Author: Vo Huu Tai
 */ 
#include "../inc/hardware_config.h"


bool bInitProgram(){
	bool ret = false;
	//IO initialize
	LED_BUB_INIT;		//Led debug is output
	KEY_INIT;			//Key is input
	SWITCH_INIT;		//Switch is input
	
	return ret;
}