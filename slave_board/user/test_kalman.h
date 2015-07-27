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

//#define BUG

#define GET_ERROR(data) (int)((0) - (data))
#define RAD_TO_DEG 57.29577951f
struct KALMAN_DATA S_Kalman_X;
uint32_t timer;

int vTestKalman(){
		
	float accY, accZ;
	float gyroX;	
	float kalAngleX = 0.0; // Calculated angle using a Kalman filter	
	uint8_t i2cData[10]; // Buffer for I2C data
	
	#ifdef BUG
	uint16_t cnt = 0;
	mpu6050_readByte(MPU6050_RA_WHO_AM_I,i2cData);
	printf("\nTest Kalman: 0x%x",i2cData[0]);
	_delay_ms(1000);	
	#endif	
	
	mpu6050_readBytes(0x3B, 6, i2cData);
	//accX = (i2cData[0] << 8) | i2cData[1];
	accY = (i2cData[2] << 8) | i2cData[3];
	accZ = (i2cData[4] << 8) | i2cData[5];
	double roll  = atan2(accY, accZ) * RAD_TO_DEG;
	
	setAngle(&S_Kalman_X,roll); // Set starting angle
	Kalman(&S_Kalman_X); 
	
	timer = sys_clk_get_msec();	
	
	//while(1)
	{
		//////////////////////////////////////////////////////////////////////////
		//Caculate data
		//////////////////////////////////////////////////////////////////////////
		
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

		//////////////////////////////////////////////////////////////////////////
		//End caculator data
		//////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////////////////
		//Debug
		//////////////////////////////////////////////////////////////////////////
		#ifdef BUG		
		if(sys_clk_get_msec() > (cnt + 100))//1ms*100 = 100ms
		{
			cnt = sys_clk_get_msec();
			
			printf("\nX: %d",GET_ERROR(kalAngleX));
		}
		#endif
		//////////////////////////////////////////////////////////////////////////
		//END Debug
		//////////////////////////////////////////////////////////////////////////
		return (int)kalAngleX;
		//_delay_us(1000);
	}//end while(1)	
}



#endif /* TEST_KALMAN_H_ */