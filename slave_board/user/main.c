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


void vUpDateLed7(struct S_UART_PACKET *packet){
	if(packet->length == 2){
		//data 0	1
		//     12	34	
		vOutLed7Seg(((int)packet->data[0] << 8) | (packet -> data[1]));
	}	
}
int main(void)
{
	struct S_UART_PACKET *command = (struct S_UART_PACKET*)malloc(sizeof(struct S_UART_PACKET));
	vInitProgram();		
	vOutLed7Seg(1235);
	while(mpu6050_testConnection() != 0x68){
		_delay_ms(100);
		//printf("\n0x%x",mpu6050_testConnection());
	}
	vOutLed7Seg(0000);
    while(1)
    {
       vTestKalman();
	   //debug
	   bDebugProcess();
	   if(b_command_avaid()){
		   command = s_get_command();
		   switch (command->info)
		   {
			   case CMD_UPDATE_LED_7SEG:
					vUpDateLed7(command);					
			   break;
			   default:
			   break;
		   }//end witch();
	   }//end if b_command_avaid
    }
}

