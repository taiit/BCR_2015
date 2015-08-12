/*
 * debug.h
 *
 * Created: 6/6/2015 10:41:53 AM
 *  Author: Vo Huu Tai
 */ 


#ifndef DEBUG_H_
#define DEBUG_H_

// [Vo Huu Tai 8/8/2015 ]  remove for optimized size
//#include <stdio.h>	//for printf
//#include <stdarg.h>
#include <stdbool.h> //for bool
#include <stdlib.h> //for malloc
#include "serial.h"
#include "common.h"

//#define _IN_FILE_BUD_
/* uart define. baud: 19200 */
#define UART_BAUD_RATE		19200UL
#define DEC_TYPE			0
#define HEC_TYPE			1
#define DEBUG_BUFF_SIZE		64
/*
 Variable
*/

struct S_UART_PACKET{
	uint8_t ucInfo;
	uint8_t ucDataLength;		//length of data
	uint8_t *ucPtrData;
	uint8_t ucCheckSum;
};
 enum E_CMD_TYPE{
	CMD_UPDATE_LED_7SEG,
	CMD_BEEP,
	CMD_SENSOR,
	CMD_NONE,
};



extern void v_init_debug_via_uart(void);
extern bool bDebugProcess(void);
extern bool bMsgIsOK();
// [Vo Huu Tai 8/8/2015 ]  Modified for new module
extern void vSetCMDInfo(uint8_t ucCMDType);
extern uint8_t ucGetCMDInfo();
extern uint8_t ucGetDataLength();
extern void ucGetData(uint8_t *ucPrtData);
// [Vo Huu Tai 8/8/2015 ]  end modified
//extern void print(const char *p, ...);// [Vo Huu Tai 8/8/2015 ]  Remove for optimized size

// [Vo Huu Tai 8/8/2015 ]  Add new print string and print int data
extern void vPutStr(char *cPtr);
extern void vPutIntNum(int iNum,uint8_t ucFomart);
extern void vSendMSG(struct S_UART_PACKET K_MSG);
// [Vo Huu Tai 8/8/2015 ]  End add

// [Vo Huu Tai 9/8/2015 ]  Add for master
extern void vOutLed7(unsigned int uiData);
extern void vBeep(unsigned int uiBeepTime);
extern void vInclinedPoll();
#endif /* DEBUG_H_ */