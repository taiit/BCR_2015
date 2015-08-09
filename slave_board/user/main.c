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
#include "test_kalman.h"


int main(void)
{
	//int data,cnt = 0;
	vInitProgram();		
	vOutLed7Seg(1235);
	while(mpu6050_testConnection() != 0x68){
		_delay_ms(100);
		//print("\n0x%x",mpu6050_testConnection());
	}
	vOutLed7Seg(0000);
    while(1)
    {
		vKalmanRuning();	  
    }
}
