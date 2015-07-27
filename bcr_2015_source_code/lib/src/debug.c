/*************************************************************************
Title:    example program for the Interrupt controlled UART library
Author:   Peter Fleury <pfleury@gmx.ch>   http://jump.to/fleury
File:     $Id: test_uart.c,v 1.5 2012/09/14 17:59:08 peter Exp $
Software: AVR-GCC 3.4, AVRlibc 1.4
Hardware: any AVR with built-in UART, tested on AT90S8515 at 4 Mhz

DESCRIPTION:
          This example shows how to use the UART library uart.c

*************************************************************************/


#include "../inc/debug.h"


//stdout for printf
static FILE usartstd = FDEV_SETUP_STREAM(uart_putc, NULL,_FDEV_SETUP_WRITE);

struct S_UART_PACKET sUartPacket;
unsigned int *ptr_rx_buff, index_rx_buf;
unsigned char *ptr_command_data;
bool bIsDubugCommandCompelete = false;


void v_init_debug_via_uart(){
	
	/*
     *  Initialize UART library for debuger, pass baud rate and AVR CPU clock
     *  with the macro 
     *  UART_BAUD_SELECT() (normal speed mode )     
     */	
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 		
	
	stdout = &usartstd;
	ptr_rx_buff = (unsigned int *)malloc(sizeof(unsigned int));
	ptr_command_data = (unsigned char *)malloc(sizeof(unsigned char));
	index_rx_buf = 0;
	#ifdef _IN_FILE_BUD_
	printf("\nFunction vInitDubugViaUART() Done");
	#endif	
}

bool bDebugProcess(void){
	
	unsigned int c;
	c = uart_getc();		
	
	if(c & UART_NO_DATA){
		return false; //nothing in uart buffer, return
	} else{
		if(c == '>'){ //first
			index_rx_buf = 0;
			bIsDubugCommandCompelete = false;
		}
		else if(c == '<'){//end
			
			sUartPacket.info = ptr_rx_buff[0];
			sUartPacket.length = ptr_rx_buff[1];
			for(uint8_t i = 0; i < sUartPacket.length; i++){
				ptr_command_data[i] = ptr_rx_buff[i+2];
			}
			sUartPacket.data = ptr_command_data;
			sUartPacket.checksum = ptr_rx_buff[sUartPacket.length + 2];
			#ifdef _IN_FILE_BUD_
			// info and length
			printf("\nPacket info %d. Packet length: %d ", sUartPacket.info
			, sUartPacket.length);
			// command data
			printf("\nRx command data: ");
			for(uint8_t i = 0; i < sUartPacket.length; i++){
				printf(" %d",sUartPacket.data[i]);
			}
			printf("\nCheck sum: %d",sUartPacket.checksum);
			#endif
		   
			bIsDubugCommandCompelete = true;
		}
		else { //get data
			bIsDubugCommandCompelete = false;
			ptr_rx_buff[index_rx_buf++] = c;			
		}		
	}	
	return false;
}

bool b_command_avaid(){
	if(bIsDubugCommandCompelete){
		if((sUartPacket.checksum | 0xff) == 0xff)return true;
	}	
	return false;
}
struct S_UART_PACKET *s_get_command(){
	return &sUartPacket;
}