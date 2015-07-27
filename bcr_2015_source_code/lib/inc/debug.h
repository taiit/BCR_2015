/*
 * debug.h
 *
 * Created: 6/6/2015 10:41:53 AM
 *  Author: Vo Huu Tai
 */ 


#ifndef DEBUG_H_
#define DEBUG_H_


#include <stdio.h>	//for printf
#include <stdbool.h> //for bool
#include <stdlib.h> //for malloc
#include "serial.h"


#ifndef F_CPU
#define F_CPU 12000000UL
#endif

//#define _IN_FILE_BUD_
/* uart define. baud: 19200 */
#define UART_BAUD_RATE    19200UL

/*
 Variable
*/

struct S_UART_PACKET{
	uint8_t info;
	uint8_t length;
	volatile uint8_t *data;
	uint8_t checksum;
};
enum E_CMD{
	CMD_UPDATE_LED_7SEG,
};
extern void v_init_debug_via_uart(void);
extern bool bDebugProcess(void);
bool b_command_avaid();

extern struct S_UART_PACKET *s_get_command(void);

#endif /* DEBUG_H_ */