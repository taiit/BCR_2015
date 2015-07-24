/*
 * unit_test_main.c
 *
 * Created: 7/20/2015 9:29:25 PM
 *  Author: Vo Huu Tai
 */ 
#include "uint_test.h"
//define 

#define UNIT_TEST_LED_BUG
//#define UNIT_TEST_KEY
//#define UNIT_TEST_SWITCH
//#define UNIT_TEST_SLAVE_BOARD

void vUnitTest(){	
		
	#ifdef UNIT_TEST_LED_BUG
	vLedDebugTest();
	#endif
	
	#ifdef UNIT_TEST_KEY
	vKeyTest();
	#endif
	
	#ifdef UNIT_TEST_SWITCH
	vSwitchTest();
	#endif	
	
	#ifdef UNIT_TEST_SLAVE_BOARD
	vSlaveTest();
	#endif
}