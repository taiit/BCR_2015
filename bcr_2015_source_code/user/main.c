/*
 * main.c
 *
 * Created: 7/20/2015 8:07:28 PM
 *  Author: Vo Huu Tai
 */ 


#include "../lib/inc/hardware_config.h"
#include "../lib/inc/hardware_feature.h"
#include "unit_test/uint_test.h"
#include "../lib/inc/LedBug.h"
#include "../lib/inc/debug.h"

volatile uint8_t ucTimer10ms = 0;
volatile uint8_t uctimer20ms = 0;
volatile uint8_t ucTimer05ms	= 0;
// [Vo Huu Tai 20/8/2015 ]  ADD Car Run
void vRuning();

// [Vo Huu Tai 20/8/2015 ]  END ADD car Run
void vAssassin(){
	// [Vo Huu Tai 10/8/2015 ]  over 1ms
	ucTimer10ms++;
	uctimer20ms++;
	ucTimer05ms++;
	if(ucTimer05ms == 5){//5ms
		scanLed7();
		ucTimer05ms = 0;
	}
	if(ucTimer10ms == 10){//10ms
		//iGetInlined();
		vLedFlash();
		ucTimer10ms = 0;
	}
	if (uctimer20ms == 20){//20ms
		#ifdef USE_PID		
			vCalPID();	
		#endif
		uctimer20ms = 0;		
	}
	
}
int main(void)
{
	int iDataTemp = 0;
	vInitProgram();	
	
	//_delay_ms(1000);//waiting for slaver ready, remove it ...
	/**
	       vOutLed7(1234);		// Xuat so 1234		   
		   vBeep(100);			// Phat ra tieng beeep 100 ms
	*/	
    vOutLed7(1234);
	vBeep(100);	
	vMotor(0,0,false);
	vLoad2PServoCenter();
	vServo(0);
	while(1)
    {       	
		if(isTester()){//Get switch tester
			switch(ucGetSwitch()&0x07){
				case 0://test sensor
					ucGetSensorData(0x7F);
					break;
				case 1://Read ADC channel
					if(bKeyIsPress(KEY1))iDataTemp++;
					if(bKeyIsPress(KEY2))iDataTemp--;
					iDataTemp&=0x7;
					vOutLed7(uiReadADC(iDataTemp));
					vOutLed1(0x80 >> (iDataTemp));
					_delay_ms(100);
					break;
				case 2://Read adc epprom
					if(bKeyIsPress(KEY3))vLoadE2PSensor();
					if(bKeyIsPress(KEY1))iDataTemp++;
					if(bKeyIsPress(KEY2))iDataTemp--;
					iDataTemp &= 0x7;
					vOutLed7(uiGetADCAverage(iDataTemp));
					vOutLed1(1 << (iDataTemp));
					break;
				case 3://ServoCenter ADJ
					//vSetServoCenter(2350);	
					vLoad2PServoCenter();
					iDataTemp = uiGetServoCenter();
					while(1){
						
						if (bKeyIsPress(KEY3))
						{
							vSetServoCenter(iDataTemp);	
							break;
						}
						if(bKeyIsPress(KEY1))iDataTemp++;
						if(bKeyIsPress(KEY2))iDataTemp--;
						vOutLed7(iDataTemp);	
						vServo(0);	
					}	
					break;
				case 4:
				break;
				case 5:
				break;
				case 6:
				break;
				case 7:
				break;
			}
			
			
			#if 0
			vOutLed7((ucGetSwitch()&0x07));//0000 0111
			bDebugProcess();
			if(bMsgIsOK()){
				switch (ucGetCMDInfo())
				{
					case CMD_SENSOR:
					vSetCMDInfo(CMD_NONE);
					ucGetData(ucBuff);
					if (ucBuff[0] == 1){//negative
						iSensorData = -ucBuff[1];
						vOutLed7((-iSensorData + 1000));
					}
					if(ucBuff[0] == 0){
						iSensorData = ucBuff[1];
						vOutLed7(iSensorData);
					}
					
					break;
					default: break;
				}
			}
			
			if(bKeyIsPress(KEY2)){
				vLedCtrl(LED_STARTUP_COMPELETE);
			}
			if(bStartBarClose()){
				vLedCtrl(LED_STARTUP_COMPELETE); //Fix me
			}
			#endif
		}//end tester
		else{//normal running
			if(bKeyIsPress(KEY2)){//Key 2 learn color, and run
				vLedCtrl(LED_SLOW_BLINK);
				vLearnColor();			
				vRuning();
			}//end Key 2 learn color, and run
			if(bKeyIsPress(KEY1)){//Load epprom and run
				vLoadE2PSensor();
				#if 0
				iSensorData = 0;
				while(1){
					if (bKeyIsPress(KEY1))
					{
						//xx xx xx  xx  xx xx xx    --
						iSensorData ++;
						if(iSensorData == 8){
							iSensorData = 0;
							vLedCtrl(LED_START_BAR_OK);
						}
					}
					//vOutLed7(read_adc(iSensorData));
					vOutLed7(ucGetSensorData(0xff));
					_delay_ms(100);
					
				}//end while 1
				#endif
				vRuning();
				
			}// end Load epprom and run
		} //end normal running
    }//end while(1)
}
// [Vo Huu Tai 20/8/2015 ]  Add for car
//prototypes
int check_crossline( uint8_t check_from  );
int check_leftline( void );
int check_rightline( void );
void followLineAfterSignal(int maxSpeed, bool PID_ON );
void turn90right( void );
void turn90left( void );
int turn90( void );
int noLine( void );
//variables

volatile uint16_t cnt_timer_1ms = 0;
int cnt_int1 = 0;
uint16_t pattern = 0;
bool PID = false;

//duong cong, ex: phai
int CS_A1[5]={
	45,//cong phai lon nhat
	45,//1
	43,//2
	45,//3
	35//4
};
int CS_A2[5]={
	-10, //cong phai lon nhat 0
	-5,//1
	18,//2
	30,//3
	30//4
};
int angle[7] = {
	13,	//0
	18, //duong cong
	20,
	30,
	40,
	45,	//5. End cong
	50
};

void vRuning(){
	//uint16_t fl = 0;
	// - 45 0 45
	pattern = CAR_IDLE;	
	//vLedCtrl(LED_SLOW_BLINK);
	while(1){
		switch (pattern)
		{
			case CAR_IDLE:
//Idle ready to run
				vOutLed7(6900);
				if(bStartBarClose())pattern = CAR_IDLE + 1;
				if(bKeyIsPress(KEY1)){
					pattern = CAR_NORMAL_TRACE;
					vOutLed7(CAR_NORMAL_TRACE);
				}
				break;
			case (CAR_IDLE + 1):
				vOutLed7(6901);
				if(bStartBarClose())LED_BUG_ON;
				else{
					pattern = CAR_NORMAL_TRACE;
					vOutLed7(CAR_NORMAL_TRACE);
					vLedCtrl(LED_STARTUP_COMPELETE);
				}
				if(bKeyIsPress(KEY1)){
					pattern = CAR_NORMAL_TRACE;
					vOutLed7(CAR_NORMAL_TRACE);
					vLedCtrl(LED_STARTUP_COMPELETE);
				}
				break;
			case CAR_NORMAL_TRACE :
				vOutLed7(CAR_NORMAL_TRACE);
// Normal trace
			PID = true;	
			//--			
			switch( ucGetSensorData(0x77) ) {	// 0 111 0 111
				case 0x00: // 0 000 0 000
					vServo( 0 );
					vMotor( 50 ,50,PID);
					break;				

				case 0x04: // 0 000 0 100
					vServo( angle[0] );
					vMotor( 45, 30,PID );
					break;		
///
				case 0x10:// 0 001 0 000
					vServo( -angle[0] );
					vMotor( 30, 45, PID );					
					break;
				default:
					vMotor(0,0,PID);
					break;
			}//end switch 0x77
			switch(ucGetSensorData(0x03)){//cong phai 0 000 0 0xx
				case 0x03:
					pattern = CAR_NORMAL_TRACE + 1;
					break;
				default:
					break;
			}//end switch 0x20
			switch(ucGetSensorData(0x60)){// cong trai. 0 xx0 0 000
				case 0x60:
					pattern = CAR_NORMAL_TRACE + 2;
					break;
				default:
					vMotor(30,30,true);
					break;
			}//end witch 0x02
			
			if (check_rightline()) {	// 0 000 1 111			
				pattern = CAR_RIGHT_LINE;
				vMotor(5,5,PID);	
				cnt_timer_1ms = 0;		
			}

			if (check_leftline()) {	// 0 111 1 000			
				pattern = CAR_LEFT_LINE;
				vMotor(5,5,PID);						
				cnt_timer_1ms = 0;
			}
				break; //end case 10
// Normal trace, cong phai		
			case (CAR_NORMAL_TRACE + 1)://cong phai
				vOutLed7(CAR_NORMAL_TRACE+1);
				PID = false;
				switch(ucGetSensorData(0x77)){ // 0 xxx 0 xxx				
					case 0x01:// 0 000 0 001
						vServo(angle[5]);//goc lon nhat						
						vMotor(CS_A1[0],CS_A2[0],PID);
						break;
					case 0x03:// 0 000 0 011 
						vServo(angle[4]);						
						vMotor(CS_A1[1],CS_A2[1],PID);
						break;
					case 0x06://0 000 0 110
						vServo(angle[3]);						
						vMotor(CS_A1[2],CS_A2[2],PID);
						break;
					case 0x04://0 000 0 100
						vServo(angle[2]);
						vMotor(CS_A1[2],CS_A2[2],PID);
						break;
					case 0x00://0 000 0 000
						vServo(angle[1]);						
						vMotor(CS_A1[3],CS_A2[3],PID);
						break;
					case 0x30://0 011 0 000
						vServo(-18);						
						vMotor(CS_A1[4],CS_A2[4],PID);
						pattern = CAR_NORMAL_TRACE;// + 1 + 5;		//return normal run
						break;
					case 0x40: //0 100 0 000
					case 0x60: //0 110 0 000
						pattern = CAR_NORMAL_TRACE + 1 + 5;			
						break;
					default:
						break;
				}//end switch 0x64
				break; //end case 11
				case CAR_NORMAL_TRACE + 1 + 5: //case 16
					vOutLed7(CAR_NORMAL_TRACE + 1 + 5);
					vMotor(40,-15,false);
					vServo(angle[6]);
					if(ucGetSensorData(0x77) == 0x00){// 0 xxx 0 xxx
						vServo(0);
						//_delay_ms(1);
						pattern = CAR_NORMAL_TRACE;
					}
					break;
			
// Normal trace, cong trai					
			case (CAR_NORMAL_TRACE + 2)://cong trai
				vOutLed7(CAR_NORMAL_TRACE+2);
				PID = false;
				switch(ucGetSensorData(0x77)){// 0 xxx 0 xxx
					case 0x40://0 100 0 000
						vServo(-angle[5]);						
						vMotor(CS_A2[0],CS_A1[0],PID);
						break;
					case 0x60://0 110 0 000
						vServo(-angle[4]);
						vMotor(CS_A2[1],CS_A1[1],PID);
						break;
					case 0x30://0 011 0 010
						vServo(-angle[3]);
						vMotor(CS_A2[2],CS_A1[2],PID);
						break;
					case 0x10://0 001 0 000
						vServo(-angle[2]);
						vMotor(CS_A2[3],CS_A1[3],PID);
						break;
					case 0x00:
						vServo(-angle[1]);
						vMotor(CS_A2[3],CS_A1[3],PID);
						break;
					case 0x04://0 000 0 100
						vServo(18);
						vMotor(CS_A2[4],CS_A1[4],PID);
						pattern = CAR_NORMAL_TRACE;// + 2 + 5;		
						break;
					case 0x01:
					case 0x03:
						pattern = CAR_NORMAL_TRACE + 2 + 5;
					break;
					default:						
						break;
				}//end switch 0x13
				break;//end case 12
			case CAR_NORMAL_TRACE + 2 + 5: //case 17
				vOutLed7(CAR_NORMAL_TRACE + 2 + 5);
				vMotor(-15,40,false);
				vServo(-angle[6]);
				if (ucGetSensorData(0x77) == 0x00){//0 xxx 0 xxx
					vServo(0);
					//_delay_ms(1);
					pattern = CAR_NORMAL_TRACE;
				}
				break;
	//Cross line
			case CAR_CROSS_LINE: //(case 20) 0 111 1 111
				vOutLed7(CAR_CROSS_LINE);	
				vSetBF(true,true);	
				if(cnt_timer_1ms > 50){//5ms
					cnt_int1 = 0;
					pattern = CAR_CROSS_LINE + 1;
				}
				break; //CAR_CROSS_LINE
			case (CAR_CROSS_LINE + 1)://on dinh				
				switch(ucGetSensorData(0x77)){//0 fff 0 fff
					case 0x00:// 0 000 x 000
						vServo(0);
						vMotor(20,20,true);
						break;
					//
					case  0x04: // 0 000 x 100
						vServo(10);
						vMotor(15,13,true);
						break;
					case 0x06: // 0 000 x 110
						vServo(15);
						vMotor(15,10,true);
						break;
					case  0x02: // 0 000 0 010
						vServo(20);
						vMotor(10,5,false);
						break;
					case 0x03: // 0 000 x 011
						vServo(25);
						vMotor(10,-10,false);
						break;
					////
					case 0x10: //0 001 x 000
						vServo(-10);
						vMotor(13,15,false);
						break;
					case 0x30: // 0 011 x 000
						vServo(-15);
						vMotor(10,15,false);
						break;
					case 0x20: // 0 010 x 000
						vServo(-20);
						vMotor(5,10,false);
						break;
					case 0x60: // 0 110 x 000
						vServo(-25);
						vMotor(10,-10,false);
						break;
				}				
				if(cnt_int1 > 150){
					pattern = CAR_CROSS_LINE + 2;
				}
				break;
			case (CAR_CROSS_LINE + 2)://nhan tin hieu va queo
				followLineAfterSignal(20,true);
				if(ucGetSensorData(0x70) == 0x70){//0 xxx 0 000
					cnt_timer_1ms = 0;
					pattern = CAR_TURN_LEFT;					
				}
				if(ucGetSensorData(0x07) == 0x07){//0 000 0 xxx
					cnt_timer_1ms = 0;
					pattern = CAR_TURN_RIGHT;
				}
				break;
//Car turn right
			case CAR_TURN_RIGHT: // 30
				vOutLed7(CAR_TURN_RIGHT);
				
				vServo(60);
				vMotor(60,-20,false);
			
				pattern = CAR_TURN_RIGHT + 1;
				cnt_timer_1ms = 0;
			break;
			case (CAR_TURN_RIGHT + 1) :
				vOutLed7(CAR_TURN_RIGHT + 1);
				if(cnt_timer_1ms > 60){
					pattern = CAR_TURN_RIGHT + 2;
				}
				break;
			case (CAR_TURN_RIGHT + 2):
				vOutLed7(CAR_TURN_RIGHT + 2);
				if(ucGetSensorData(0x77) == 0x02){ // 0 xxx 0 xxx. 0 000 0 011
					pattern = CAR_NORMAL_TRACE;
					cnt_timer_1ms = 0;
					vSetBF(false,false);
				}
				break;
//Car turn left
			case CAR_TURN_LEFT: // 30
				vOutLed7(CAR_TURN_LEFT);
				
				vServo(-60);
				vMotor(-20,60,false);
				
				pattern = CAR_TURN_LEFT + 1;
				cnt_timer_1ms = 0;
				break;
			case (CAR_TURN_LEFT + 1) :
				if(cnt_timer_1ms > 60){
					pattern = CAR_TURN_LEFT + 2;
				}
				break;
			case (CAR_TURN_LEFT + 2):
				vOutLed7(CAR_TURN_LEFT + 2);
				if(ucGetSensorData(0x77) == 0x60){ // 0 xxx 0 xxx
					pattern = CAR_NORMAL_TRACE;
					cnt_timer_1ms = 0;
					vSetBF(false,false);
				}
				break;
//Right line - chuyen lan		
			case CAR_RIGHT_LINE://(case 51) 0 000 1 111
				vOutLed7(CAR_RIGHT_LINE);
				while(cnt_timer_1ms < 1);//1ms
				if(check_crossline(CHECK_FROM_RIGHT)){
					cnt_timer_1ms = 0;
					pattern = CAR_CROSS_LINE;
				}
				if(cnt_timer_1ms > 50){//50ms
					pattern = CAR_RIGHT_LINE + 1;
					vLedCtrl(LED_STARTUP_COMPELETE);
					cnt_int1 = 0;
				}
				break;	
			case (CAR_RIGHT_LINE + 1)://is right line, chuyen lan trai
				vOutLed7(CAR_RIGHT_LINE + 1);
				vSetBF(true,true);
				followLineAfterSignal(60,true);
				if(cnt_int1 > 140){
					pattern = CAR_RIGHT_LINE + 2;
					vSetBF(false,false);
				}
				break;
			case (CAR_RIGHT_LINE + 2):
				vOutLed7(CAR_RIGHT_LINE+2);
				vServo(38);
				vMotor(15,15,true);
				if(ucGetSensorData(0x01) == 0x01){//0 000 0 00x
					vServo(10);
					pattern = CAR_RIGHT_LINE + 3;
				}
				break;
			case (CAR_RIGHT_LINE + 3):
				vOutLed7(CAR_RIGHT_LINE+3);
				vMotor(20,20,true);
				if(ucGetSensorData(0x08) == 0x08){//0 000 x 000
					vServo(-20);
					cnt_timer_1ms = 0;
					while(cnt_timer_1ms < 1);
					pattern = CAR_RIGHT_LINE + 4;
				}
				break;
			case (CAR_RIGHT_LINE + 4):
				vOutLed7(CAR_RIGHT_LINE+4);
				vServo(-30);
				while(cnt_timer_1ms < 5);
				pattern = CAR_NORMAL_TRACE;
				break;				
			
//Left line			
			case CAR_LEFT_LINE:// (case 61) 0 111 1 000	
				vOutLed7(CAR_LEFT_LINE);
				while(cnt_timer_1ms < 1);//1ms	
				//check cross line
				if(check_crossline(CHECK_FROM_LEFT)){
					cnt_timer_1ms = 0;
					pattern = CAR_CROSS_LINE;
				}
				if(cnt_timer_1ms > 50){//50ms
					vLedCtrl(LED_STARTUP_COMPELETE);
					pattern = CAR_LEFT_LINE + 1;
					cnt_int1 = 0;
				}
				break;

			case (CAR_LEFT_LINE + 1)://is right line, chuyen lan trai				
				vOutLed7(CAR_LEFT_LINE + 1);
				vSetBF(true,true);
				followLineAfterSignal(60,true);
				if(cnt_int1 > 140){
					pattern = CAR_LEFT_LINE + 2;
					vSetBF(false,false);
				}
				break;
			case (CAR_LEFT_LINE + 2):
				vOutLed7(CAR_LEFT_LINE + 2);
				vServo(-38);
				vMotor(15,15,true);
				if(ucGetSensorData(0x40) == 0x40){//0 x00 0 000
					vServo(-10);
					pattern = CAR_LEFT_LINE + 3;
				}
				break;
			case (CAR_LEFT_LINE + 3):
				vOutLed7(CAR_LEFT_LINE + 3);
				vMotor(20,20,true);
				if(ucGetSensorData(0x08) == 0x08){//0 000 x 000
					vServo(20);
					cnt_timer_1ms = 0;
					while(cnt_timer_1ms < 1);
					pattern = CAR_LEFT_LINE + 4;
				}
				break;
			case (CAR_LEFT_LINE + 4):
				vOutLed7(CAR_LEFT_LINE + 4);
				vServo(30);
				while(cnt_timer_1ms < 5);
				pattern = CAR_NORMAL_TRACE;
				break;
			default:
			/* When the pattern is not applied to any case, return to waiting state */
			vOutLed7(0);
			vLedCtrl(LED_FAST_BLINK);
			while(1);
			pattern = 0;
			break;
		}//end switch case
	}//end while 1
}// end vRuning
// [Vo Huu Tai 20/8/2015 ]  End add for car
/************************************************************************/
/* Crossline detection processing */
/* Return value 0: no crossline 1: crossline exists */
/************************************************************************/
int check_crossline( uint8_t check_from )
{
	
	int ret = 0;
	if(check_from == CHECK_FROM_LEFT){// (case 61) 0 111 1 000		
		if(ucGetSensorData(0x0) == 0x01)ret = 1;// 0 000 0 00x
	}
	if(check_from == CHECK_FROM_RIGHT){//(case 51) 0 000 1 111
		if(ucGetSensorData(0x40) == 0x40)ret = 1;// 0 x00 0 000
	}
	return ret;
}

void followLineAfterSignal( int maxSpeed, bool PID_ON  )
{
	//if(BF_ON)vSetBF(true,true);		

	switch(ucGetSensorData(0x77)){//0 fff 0 fff
		case 0x00:// 0 000 x 000
		vServo(0);
		vMotor(maxSpeed,maxSpeed,PID_ON);
		break;
		//
		case  0x04: // 0 000 x 100
			vServo(10);
			vMotor(maxSpeed,maxSpeed-5,PID_ON);
			break;
		case 0x06: // 0 000 x 110
			vServo(15);
			vMotor(maxSpeed,maxSpeed-5,PID_ON);
			break;
		case  0x02: // 0 000 0 010
			vServo(20);
			vMotor(maxSpeed-5,maxSpeed-10,PID_ON);
			break;
		case 0x03: // 0 000 x 011
			vServo(25);
			vMotor(maxSpeed-10,maxSpeed-15,PID_ON);
			break;
		////
		case 0x10: //0 001 x 000
			vServo(-10);
			vMotor(maxSpeed-5,maxSpeed,PID_ON);
			break;
		case 0x30: // 0 011 x 000
			vServo(-15);
			vMotor(maxSpeed-5,maxSpeed,PID_ON);
			break;
		case 0x20: // 0 010 x 000
			vServo(-20);
			vMotor(maxSpeed-10,maxSpeed-5,PID_ON);
			break;
		case 0x60: // 0 110 x 000
			vServo(-25);
			vMotor(maxSpeed-15,maxSpeed-10,PID_ON);
			break;
	}
	
	//vSetBF(false,false);
	
}

/************************************************************************/
/* Check Right Line Signal                                            */
/* 0: no signal, 1: right line found                                    */
/************************************************************************/
int check_rightline( void )
{
	 int ret;

	 ret = 0;
	 if( ucGetSensorData(0x0f)==0x0f)  {// 0 000 1 111
		 ret = 1;
	 }
	 return ret;
}

/************************************************************************/
/* Check Left Line Signal                                            */
/* 0: no signal, 1: left line found   								*/
/************************************************************************/
int check_leftline( void )
{
	int ret;

	ret = 0;
	if (ucGetSensorData(0x78)==0x78) {// 0 xxx x 000
		ret = 1;
	}
	return ret;
}

// [Vo Huu Tai 20/8/2015 ]  ISR here
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 68;	//~1ms
	//Add more cnt here..
	cnt_timer_1ms++;
	vAssassin();
}
volatile int cnt_temp = 0;
ISR(INT1_vect){//encoder interrupt
	vIncPulse();
	cnt_int1++;
	if((ucGetSwitch()&0x1) == 0x1)
	vOutLed7(cnt_temp++);
}	
// [Vo Huu Tai 20/8/2015 ]  END ISR