/*
 * test_kalman.h
 *
 * Created: 6/20/2015 8:37:13 PM
 *  Author: Vo Huu Tai
 */ 


#ifndef TEST_KALMAN_H_
#define TEST_KALMAN_H_
#include "../lib/mpu6050/Kalman.h"
#include "../lib/mpu6050/mpu6050.h"
#include "../lib/inc/led7seg.h"

#define KALMAN_RUNING

#define GET_ERROR(data) (int)((0) - (data))
#define RAD_TO_DEG 57.29577951f
struct KALMAN_DATA S_Kalman_X;
uint32_t timer;
float accY, accZ;
float gyroX;
float kalAngleX = 0.0; // Calculated angle using a Kalman filter
uint8_t i2cData[10]; // Buffer for I2C data
float roll;

#ifndef KALMAN_RUNING
void vInitKalman(){
	mpu6050_readBytes(0x3B, 6, i2cData);
	//accX = (i2cData[0] << 8) | i2cData[1];
	accY = (i2cData[2] << 8) | i2cData[3];
	accZ = (i2cData[4] << 8) | i2cData[5];
	roll  = atan2(accY, accZ) * RAD_TO_DEG;
	
	setAngle(&S_Kalman_X,roll); // Set starting angle
	Kalman(&S_Kalman_X);
	
}
#endif

void vKalmanRuning(){		
		
	#ifdef KALMAN_RUNING
	uint16_t cnt = 0;
	uint8_t *ucPrtData = (uint8_t *)calloc(DEBUG_BUFF_SIZE,sizeof(uint8_t));	
	//for(cnt = 0; cnt < 8; cnt++){
		//ucPrtData[cnt] = 49;
	//}
	struct S_UART_PACKET *command = (struct S_UART_PACKET*)malloc(sizeof(struct S_UART_PACKET));
	struct S_UART_PACKET dataSend;
	//mpu6050_readByte(MPU6050_RA_WHO_AM_I,i2cData);
	//vPutStr("\nI am: ");
	//vPutIntNum(i2cData[0],HEC_TYPE);
	//_delay_ms(1000);		
	mpu6050_readBytes(0x3B, 6, i2cData);
	//accX = (i2cData[0] << 8) | i2cData[1];
	accY = (i2cData[2] << 8) | i2cData[3];
	accZ = (i2cData[4] << 8) | i2cData[5];
	roll  = atan2(accY, accZ) * RAD_TO_DEG;
	
	setAngle(&S_Kalman_X,roll); // Set starting angle
	Kalman(&S_Kalman_X);
	
	timer = sys_clk_get_msec();
	while(1)	
	{//it is main runing now...
	#endif
		//////////////////////////////////////////////////////////////////////////
		//Caculate data
		//////////////////////////////////////////////////////////////////////////
		if(sys_clk_get_msec() > (cnt + 5))//1ms*5 = 5ms
		{
			cnt = sys_clk_get_msec();
			mpu6050_readBytes(0x3B, 10, i2cData);
			//accX = ((i2cData[0] << 8) | i2cData[1]);
			accY = ((i2cData[2] << 8) | i2cData[3]);
			accZ = ((i2cData[4] << 8) | i2cData[5]);
			gyroX = (i2cData[8] << 8) | i2cData[9];		
			// gyroZ = (i2cData[12] << 8) | i2cData[13];
			float dt = (float)(sys_clk_get_msec() - timer) / 1000; // Calculate delta time
			timer = sys_clk_get_msec();
		
			roll  = atan2(accY, accZ) * RAD_TO_DEG;
			double gyroXrate = gyroX / 131.0; // Convert to deg/s
			// This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
			if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
				setAngle(&S_Kalman_X,roll);
				kalAngleX = roll;
			} else
			kalAngleX = getAngle(&S_Kalman_X,roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
		}
		//////////////////////////////////////////////////////////////////////////
		//End caculator data
		//////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////////////////
		//Debug
		//////////////////////////////////////////////////////////////////////////
		#ifdef KALMAN_RUNING	
		#if 0	
		if(sys_clk_get_msec() > (cnt + 1))//1ms*1 = 1ms
		#endif
		{
			//cnt = sys_clk_get_msec();
			//vPutStr("X: ");
			//vPutIntNum(GET_ERROR(kalAngleX),DEC_TYPE);		
			//vSendMSG(command);	
			bDebugProcess();
			if(bMsgIsOK()){
				command = S_GET_CMD_PACKET();				
				if((command != NULL)){					
					switch (command->ucInfo)
					{
						case CMD_UPDATE_LED_7SEG:
							vOutLed7Seg(command->ucPtrData[0] * 100 + command->ucPtrData[1]);
							vClearUARTBuffer(command);
							break;
						case CMD_BEEP:
							vBeepSlaver(command->ucPtrData[0] * 100 + command->ucPtrData[1]);
							vClearUARTBuffer(command);
							break;
						case CMD_SENSOR:	//send kalAngleX to master board
							dataSend.ucInfo = CMD_SENSOR;
							dataSend.ucDataLength = 2;
							kalAngleX = -kalAngleX;
							if (kalAngleX < 0){ //negative
								ucPrtData[0] = 1;
								ucPrtData[1] = -kalAngleX;
							}
							else{
								ucPrtData[0] = 0;
								ucPrtData[1] = kalAngleX;
							}
							dataSend.ucPtrData = ucPrtData;
							vSendMSG(dataSend);
							vClearUARTBuffer(command);
						default:
						//vPutIntNum(ucGetCMDInfo(),DEC_TYPE);
						break;
					}//end witch();
				}
				
			}//end if bMsgIsOK         
			
		}
		#endif
		//////////////////////////////////////////////////////////////////////////
		//END Debug
		//////////////////////////////////////////////////////////////////////////	
		
	#ifdef KALMAN_RUNING
	}//end while(1)	
	#endif	
	//return GET_ERROR(kalAngleX);	
}

int getAngleX(){
	return (GET_ERROR(kalAngleX));
}


#endif /* TEST_KALMAN_H_ */