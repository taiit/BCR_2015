/********************************************************************
	created:	2015/08/10
	created:	10:8:2015   19:00
	filename: 	\bcr_2015_source_code\lib\src\debug.c
	file path:	\bcr_2015_source_code\lib\src
	file base:	debug
	file ext:	c
	author:		Vo Huu Tai
	
	purpose:	Use this module for transfer data via UART protocol.
*********************************************************************/
#include "../inc/debug.h"


//stdout for printf
//static FILE usartstd = FDEV_SETUP_STREAM(uart_putc, NULL,_FDEV_SETUP_WRITE);

volatile struct S_UART_PACKET sUartPacket;
unsigned int *ptr_rx_buff, index_rx_buf;
unsigned char *ptr_command_data;
bool bIsDubugCommandCompelete = false;
unsigned int uiOldDataLed7seg = 0;
/*
	@brief:		Initialize UART protocol and global variable
	@param:		none
	@return:	none 
*/
PUBLIC void v_init_debug_via_uart(){
	
	/*
     *  Initialize UART library for debugger, pass baud rate and AVR CPU clock
     *  with the macro 
     *  UART_BAUD_SELECT() (normal speed mode )     
     */	
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); 		
	
	//stdout = &usartstd;
	ptr_rx_buff = (unsigned int *)calloc(DEBUG_BUFF_SIZE,sizeof(unsigned int));
	ptr_command_data = (uint8_t *)calloc(DEBUG_BUFF_SIZE,sizeof(uint8_t));
	index_rx_buf = 0;
	#ifdef _IN_FILE_BUD_
	vPutStr("vInitDubugViaUART()\n");
	#endif	
}
/*
	@brief:		This function have been handle of main function
	@param:		none
	@return:	none 
*/
PUBLIC bool bDebugProcess(void){
	
	// [Vo Huu Tai 8/8/2015 ]  Note!! unsigned int only, i don't know why ?? .. :'(
	unsigned int ucUartDataIn;
	ucUartDataIn = uart_getc();	

	if(ucUartDataIn & UART_NO_DATA){
		return false; //nothing in UART buffer, return
	} else{
		#ifdef _IN_FILE_BUD_
		//uart_putc(ucUartDataIn);
		#endif
		if(ucUartDataIn == '>'){ //first UART data incoming
			index_rx_buf = 0;
			bIsDubugCommandCompelete = false;
		}
		else if(ucUartDataIn == '<'){//end UART data coming
			
			sUartPacket.ucInfo = ptr_rx_buff[0];
			sUartPacket.ucDataLength = ptr_rx_buff[1];
			
			for(uint8_t i = 0; i < sUartPacket.ucDataLength; i++){
				ptr_command_data[i] = ptr_rx_buff[i+2];
			}
			sUartPacket.ucPtrData = ptr_command_data;
			sUartPacket.ucCheckSum = ptr_rx_buff[sUartPacket.ucDataLength + 2];
			
			#ifdef _IN_FILE_BUD_
			// info and length
			vPutStr("\n========================\n");
			vPutStr("Packet Info: "); vPutIntNum(sUartPacket.ucInfo,DEC_TYPE);
			vPutStr("Data Length: "); vPutIntNum(sUartPacket.ucDataLength,DEC_TYPE);
			vPutStr("Check sum: ");	vPutIntNum(sUartPacket.ucCheckSum,DEC_TYPE);
			// command data
			vPutStr("Data\n");
			for(uint8_t i = 0; i < sUartPacket.ucDataLength; i++){
				vPutIntNum(sUartPacket.ucPtrData[i],DEC_TYPE);
			}
			vPutStr("========================\n");			
			#endif		   
			bIsDubugCommandCompelete = true;
		}
		else { //get data
			bIsDubugCommandCompelete = false;
			ptr_rx_buff[index_rx_buf++] = ucUartDataIn;			
		}		
	}	

	return false;
}
/*
	@brief:		Check msg is OK
	@param:		none
	@return:	True: msg ready be used 
*/
PUBLIC bool bMsgIsOK(){
	if(bIsDubugCommandCompelete){
		if((sUartPacket.ucCheckSum | 0xff) == 0xff)return true;
	}	
	return false;
}
/*
	@brief:		Get command info
	@param:		none
	@return:	command info 
*/
PUBLIC uint8_t ucGetCMDInfo(){
	return sUartPacket.ucInfo;
}
/*
	@brief:		Set CMD info
	@param:		command info
	@return:	none 
*/
PUBLIC void vSetCMDInfo(uint8_t ucCMDType){
	sUartPacket.ucInfo = ucCMDType;
}
/*
	@brief:		Get data length of S_UART_PACKET
	@param:		none
	@return:	data length 
*/
PUBLIC uint8_t ucGetDataLength(){
	return sUartPacket.ucDataLength;
}
/*
	@brief:		Get data of S_UART_PACKET
	@param:		pointer to saving data
	@return:	none 
*/
PUBLIC void ucGetData(uint8_t *ucPrtData){
	// [Vo Huu Tai 9/8/2015 ]  why
	//ucPrtData = sUartPacket.ucPtrData; //why, it not work
	for (int i = 0; i < sUartPacket.ucDataLength; i++)
	{
		ucPrtData[i] = sUartPacket.ucPtrData[i];
	}
	#ifdef _IN_FILE_BUD_
	vPutStr("ucGetData() START\n");	
	// info, length, checksum
	vPutStr("Packet Info: "); vPutIntNum(sUartPacket.ucInfo,DEC_TYPE);
	vPutStr("Data Length: "); vPutIntNum(sUartPacket.ucDataLength,DEC_TYPE);
	vPutStr("Check sum: ");	vPutIntNum(sUartPacket.ucCheckSum,DEC_TYPE);
	// command data
	vPutStr("Data\n");
	for(uint8_t i = 0; i < sUartPacket.ucDataLength; i++){
		vPutIntNum(ucPrtData[i],DEC_TYPE);
	}	
	vPutStr("ucGetData() END\n");
	#endif
}

// [Vo Huu Tai 8/8/2015 ]  Remove for optimized size
#if 0
void print(const char *p, ...){
	va_list list;
	unsigned int num, digit;
	int i;
	int zeroPadding = 0;
	int formatLen = 0;
	int base;
	int minusF;
	char numStack[11]; 

	va_start(list, p);
	while (*p) {
		switch (*p) {
			case '%':
				zeroPadding = 0;
				if (p[1] == '0') {
					zeroPadding = 1;++p;
				}
				formatLen = 0;
				while (*++p) { 
					switch (*p) {
						case '%':
							uart_putc(*p);
							goto NextLoop0;
						case 'c':
							uart_putc(va_arg(list, int));
							goto NextLoop0;
						case 'd':
						case 'X':
						case 'x':
							minusF = 0;
							num = va_arg(list, unsigned int);
							if (*p == 'd') {
								base = 10;
								if (num & 0x80000000) {
									num = -(signed int)num;
									minusF = 1;
								}
								} else {
								base = 16; /* FixMe */
							}
							for (digit = 0; digit < sizeof(numStack);) {
								numStack[digit++] = num%base;
								num /= base;
								if (num == 0) break;
							}
							if (minusF) numStack[digit++] = 0x7F;/*FixMe*/
							if (formatLen > digit) {
								char paddingChar = ' ';
								formatLen -= digit;
								if (zeroPadding) paddingChar = '0';
								while (formatLen--) {
									uart_putc(paddingChar);
								}
							}
							for (i = digit-1; i >= 0; i--) {
								if (numStack[i] == 0x7F) {
									uart_putc('-');
									} else if (numStack[i] > 9) {
									uart_putc(numStack[i]-10 + 'A');
									} else {
									uart_putc(numStack[i] + '0');
								}
							}
							goto NextLoop0;
						default:
							if (*p >= '0' && *p <= '9') {
								formatLen = formatLen*10 + (*p-'0');
								} else {
								goto EXIT_LABEL;/*Error*/
							}
					}
				}
				if (*p == 0) goto EXIT_LABEL;/*Error*/
				break;
			default:
				uart_putc(*p);
		}
		NextLoop0:
		p++;
	}
	EXIT_LABEL:
	va_end(list);
}
#endif
// [Vo Huu Tai 8/8/2015 ]  end remove

// [Vo Huu Tai 8/8/2015 ]  add new for print and send msg
/*
	@brief:		Internal function. Output a 32-bit number in unsigned decimal format
	@param:		unsigned number to be printed
	@return:	none 
*/
PRIVATE void vOutUDec(unsigned int uiNum){
	// This function uses recursion to convert decimal number
	//   of unspecified length as an ASCII string
	if(uiNum >= 10){
		vOutUDec(uiNum / 10);
		uiNum = uiNum % 10;
	}
	uart_putc('0' + uiNum); /* n is between 0 and 9 */
}
/*
	@brief:		Internal function. Output a 32-bit number in unsigned hexadecimal format
	@param:		Unsigned int number to be transferred
	@return:	None
*/
PRIVATE void vOutUHex(unsigned int uiNum){
	// This function uses recursion to convert the number of
	//   unspecified length as an ASCII string
	if(uiNum >= 0x10){
		vOutUHex(uiNum / 0x10);
		vOutUHex(uiNum % 0x10);
	}
	else{
		if(uiNum < 0xA){
			uart_putc(uiNum + '0');
		}
		else{
			uart_putc((uiNum - 0x0A) + 'A');
		}
	}
}
/*
	@brief:		Internal function, Cal checksum
	@param:		UART PACKET
	@return:	Check sum of data  
	@Note:		Mine, check sum for only data, not for type and length,..
*/
PRIVATE uint8_t ucCalChecSum(struct S_UART_PACKET sPacket){
	
	int ucRet = 0;
	
	ucRet += sPacket.ucInfo;
	ucRet += sPacket.ucDataLength;
	for (int i = 0; i < sPacket.ucDataLength; i++)
	{
		 ucRet += sPacket.ucPtrData[i];
	}
	 ucRet ^= 0xFF;
	 return (uint8_t) ucRet;
	
}
/*
	@brief:		Print string
	@param:		string to print
	@return:	none 
*/
PUBLIC void vPutStr(char *cPtr){
	while(*cPtr){		
		if(*cPtr == '\n')uart_putc('\r');
		uart_putc(*cPtr);
		cPtr++;
	}	
}
/*
	@brief:		Print int data
	@param:		Number type int to be printed, Hex or Dec format
	@return:	none 
*/
PUBLIC void vPutIntNum(int iNum,uint8_t ucFomart){	
		
	if(iNum < 0){	//negate
		uart_putc('-');
		iNum = - iNum;
		ucFomart = DEC_TYPE; //if Negate, format always is DEC_TYPE
	}
	if(ucFomart == 0){	//Decimal format
		
		vOutUDec(iNum);
	}
	else{				//Hexadecimal
		uart_putc('0');
		uart_putc('x');
		vOutUHex(iNum);
	}	
	uart_putc('\r');
	uart_putc('\n');	
}
/*
	@brief:		Sending msg via UART
	@param:		msg to send
	@return:	none 
*/
PUBLIC void vSendMSG(struct S_UART_PACKET K_MSG){
	#if 0
	vPutStr("vSendMSG() START\n");
	// info, length, checksum
	vPutStr("Packet Info: "); vPutIntNum(K_MSG.ucInfo,DEC_TYPE);
	vPutStr("Data Length: "); vPutIntNum(K_MSG.ucDataLength,DEC_TYPE);
	vPutStr("Check sum: ");	vPutIntNum(K_MSG.ucCheckSum,DEC_TYPE);
	// command data
	vPutStr("Data\n");
	for(uint8_t i = 0; i < K_MSG.ucDataLength; i++){
		vPutIntNum(K_MSG.ucPtrData[i],DEC_TYPE);
	}
	vPutStr("vSendMSG() END\n");
	#endif
	/**
	UART msg format
	
	       >	type	datalength		data[0:length]	checksum	<
		   
		   >	0			1			48				xx		<
	
	*/
	uint8_t *ucPtrDataSend = (uint8_t*)calloc(DEBUG_BUFF_SIZE,sizeof(uint8_t));
	
	ucPtrDataSend[0] = '>';
	ucPtrDataSend[1] = K_MSG.ucInfo;
	ucPtrDataSend[2] = K_MSG.ucDataLength;
	
	for(int i = 0; i < K_MSG.ucDataLength; i++){
		ucPtrDataSend[3 + i] = K_MSG.ucPtrData[i];
	}
	
	ucPtrDataSend[3 + K_MSG.ucDataLength] = ucCalChecSum(K_MSG);
	ucPtrDataSend[4 + K_MSG.ucDataLength] = '<';	
	
	
	for(uint8_t i = 0; i < (K_MSG.ucDataLength + 5); i++){
		uart_putc(ucPtrDataSend[i]);
	}
	
	free(ucPtrDataSend);
}
// [Vo Huu Tai 8/8/2015 ]  end add new
/*
	@brief:		Send data data' LED 7 segment to slave board
	@param:		Data to be sent
	@return:	none 
*/
PUBLIC void vOutLed7(unsigned int uiData){
	if(uiData == uiOldDataLed7seg)return;
	
	
	struct S_UART_PACKET msg ;
	uint8_t ucPrtData[2];
	
	ucPrtData[0] = uiData / 100;
	ucPrtData[1] = uiData % 100;
	
	msg.ucInfo = CMD_UPDATE_LED_7SEG;
	msg.ucDataLength = 2;
	msg.ucPtrData = ucPrtData;	
	
	vSendMSG(msg);	
	
	uiOldDataLed7seg = uiData;
}
/*
	@brief:		send msg control beep
	@param:		time beep in ms, form 000 to 9999 ms
	@return:	none 
*/
PUBLIC void vBeep(unsigned int uiBeepTime_ms){
	uint8_t ucPrtData[2];
	struct S_UART_PACKET msg ;
	
	if(uiBeepTime_ms > 9999)uiBeepTime_ms = 9999;
	
	ucPrtData[0] = uiBeepTime_ms / 100;
	ucPrtData[1] = uiBeepTime_ms % 100;

	
	msg.ucInfo = CMD_BEEP;
	msg.ucDataLength = 2;
	msg.ucPtrData = ucPrtData;
	
	vSendMSG(msg);	
}
/*
	@brief:		Get inclined of mpu6050 board
	@param:		none
	@return:	inclined of sensor 
*/
PUBLIC int iGetInlined(){
	uint8_t ucPrtData[1] = {0};// = (uint8_t *)calloc(1,sizeof(uint8_t));	
	
	struct S_UART_PACKET msg ;
	msg.ucInfo = CMD_SENSOR;
	msg.ucDataLength = 1;
	msg.ucPtrData = ucPrtData;
	vSendMSG(msg);	
		
	return 0;
}