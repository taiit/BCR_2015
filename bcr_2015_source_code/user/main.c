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

volatile uint16_t uiTimer0Cnt = 0;
// [Vo Huu Tai 20/8/2015 ]  ADD Car Run
void vRuning();

// [Vo Huu Tai 20/8/2015 ]  END ADD car Run
void vAssassin(){
	// [Vo Huu Tai 10/8/2015 ]  over 1ms
	uiTimer0Cnt++;
	switch(uiTimer0Cnt){//I wish it is called very 10ms		
		case 10:
			iGetInlined();
			vLedFlash();
			uiTimer0Cnt = 0;
			break;
	}
	// [Vo Huu Tai 14/8/2015 ]  BUGGY here :(((
}
int main(void)
{
	uint8_t *ucBuff = (uint8_t*)calloc(DEBUG_BUFF_SIZE,sizeof(uint8_t));
	int iSensorData = 0;
	vInitProgram();	
	
	_delay_ms(1000);//waiting for slaver ready, remove it ...
	/**
	       vOutLed7(1234);		// Xuat so 1234		   
		   vBeep(100);			// Phat ra tieng beeep 100 ms
	*/	
    vOutLed7(1234);
	vBeep(100);	
	vMotor(0,0);
	vServo(0);
	while(1)
    {       	
		if(isTester()){//Get switch tester
			vOutLed7(ucGetSwitch());
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
			
			if(bKeyIsPress(KEY1)){
				vLedCtrl(LED_STARTUP_COMPELETE);
			}
			if(bStartBarIsStart()){
				vLedCtrl(LED_STARTUP_COMPELETE); //Fix me
			}
		}//end tester
		else{//normal running
			if(bKeyIsPress(KEY2)){//Key 2 learn color, and run
				vLearnColor();			
				vRuning();
			}//end Key 2 learn color, and run
			if(bKeyIsPress(KEY1)){//Load epprom and run
				vLoadE2P();
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
int check_leftline( void );
int check_rightline( void );
void followLineAfterSignal( void );
void turn90right( void );
void turn90left( void );
int turn90( void );
int noLine( void );
//variables

uint16_t s_cnt = 0;
uint16_t pattern = 0;
uint16_t crossline = 0;
int vMotor_BL[5] = {
	35,
	20,
	15,
	0,
	0
};
int vMotor_BR[5] = {
	35,			//0
	20,			//1
	15,			//2
	0,			//3
	0			//4
};
int angle[6] = {
	15,		//0
	25,//duong cong
	30,
	45,
	50,
	55		//5
};
void vRuning(){
	//uint16_t fl = 0;
	// - 45 0 45
	vOutLed7(9999);
	pattern = 10;
	//SLAVE_OFF;
	while(1){
		switch (pattern)
		{
			case 10 :
			   /* Normal trace */
				
			//--
			switch( ucGetSensorData(0x77) ) {	// 0 111 0 111
				case 0x00: // 0 000 0 000
					vServo( 0 );
					vMotor( vMotor_BL[0] ,vMotor_BR[0] );
					break;				
///
				case 0x04: // 0 000 0 100
					vServo( angle[0] );
					vMotor( vMotor_BL[0], vMotor_BR[1] );
					break;		
///
				case 0x10:// 0 001 0 000
					vServo( -angle[0] );
					vMotor( vMotor_BL[1] , vMotor_BR[0] );					
					break;
				default:
					vMotor(0,0);
					break;
			}//end switch 0x77
			switch(ucGetSensorData(0x02)){//cong phai 0 000 0 0x0
				case 0x02:
					pattern = 11;
					break;
				default:
					break;
			}//end switch 0x20
			switch(ucGetSensorData(0x20)){// cong trai. 0 0x0 0 000
				case 0x20:
					pattern = 12;
					break;
				default:
					break;
			}//end witch 0x02
				break; //end case 10
				
			case 11://cong phai
				switch(ucGetSensorData(0x64)){ // 0 xx0 0 x00				
					case 0x40:// 0 100 0 000
						vServo(angle[5]);
						vMotor(30,-10);
						break;
					case 0x60:// 0 110 0 000 
						vServo(angle[4]);
						vMotor(30,0);
						break;
					case 0x20://0 010 0 000
						vServo(angle[3]);
						vMotor(27,10);
						break;
					case 0x00://0 000 0 000
						vServo(angle[2]);
						vMotor(25,20);
						break;
					case 0x04://0 000 0 100
						vServo(angle[0]);
						vMotor(30,25);
						pattern = 10;		//return normal run
						break;
					default:
						pattern = 11;
						break;
				}//end switch 0x64
				break; //end case 11
			case 12://cong trai
				switch(ucGetSensorData(0x13)){// 0 00x 0 0xx
					case 0x01://0 000 0 001
						vServo(-angle[5] - 10);
						vMotor(-10,30);
						break;
					case 0x03://0 000 0 011
						vServo(-angle[4] - 9);
						vMotor(0,30);
						break;
					case 0x02://0 000 0 010
						vServo(-angle[3] - 8);
						vMotor(10,27);
						break;
					case 0x00://0 000 0 000
						vServo(-angle[2] - 7);
						vMotor(20,25);
						break;
					case 0x10://0 001 0 000
						vServo(-angle[1] - 6);
						vMotor(25,30);
						pattern = 10;		//return normal run
						break;
					default:
						pattern = 12;
						break;
				}//end switch 0x13
				break;//end case 12

			case 21:			
				break; //end case 21

			case 22:			
				break;//end case 22
			
			case 51:
				break;	

			case 52:			
				break;
			
			case 53:
				break;
			
			case 54:			
				break;
			
			case 61:			
				break;

			case 62:
				break;
			
			case 63:
				break;
			
			case 64:
				break;

			case 31:
				break;

			case 41:			
				break;
			
			default:
			/* When the pattern is not applied to any case, return to waiting state */
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
int noLine( void )
{
	return 0;
}

int turn90( void )
{
	return 0;
}

void turn90left( void )
{
	
}

void turn90right( void )
{
	
}

void followLineAfterSignal( void )
{
	switch( ucGetSensorData(0x77)) {
		case 0x00:
			vServo( 0 );
			vMotor(70,70);
			break;
		case 0x04:
		case 0x06:
		case 0x07:
		case 0x03:
			vServo( 5 );
			vMotor(70,60);
			break;
		case 0x10:
		case 0x30:
		case 0x70:
		case 0x60:
			vServo( -5 );
			vMotor(60,70);
			break;
	}
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
	if (ucGetSensorData(0x78)==0x78) {// 0 111 1 000
		ret = 1;
	}
	return ret;
}

// [Vo Huu Tai 20/8/2015 ]  ISR here
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 68;	//~1ms
	//Add more cnt here..
	vAssassin();
}
// [Vo Huu Tai 20/8/2015 ]  END ISR