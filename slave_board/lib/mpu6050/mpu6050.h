/*
MPU6050 lib 0x02

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.

References:
  - most of the code is a port of the arduino mpu6050 library by Jeff Rowberg
    https://github.com/jrowberg/i2cdevlib
  - Mahony complementary filter for attitude estimation
    http://www.x-io.co.uk
*/


#ifndef MPU6050_H_
#define MPU6050_H_

#include <avr/io.h>
#include "mpu6050registers.h"

//i2c settings
#define MPU6050_I2CFLEURYPATH "../i2chw/i2cmaster.h" //define the path to i2c fleury l	ib

//definitions
#define MPU6050_ADDR (0x68 <<1) //device address - 0x68 pin low (GND), 0x69 pin high (VCC)

//definitions for raw data
//gyro and acc scale
#define MPU6050_GYRO_FS MPU6050_GYRO_FS_2000
#define MPU6050_ACCEL_FS MPU6050_ACCEL_FS_2
//////////////////////////////////////////////////////////////////////////

#define RADIANS_TO_DEGREES 57.29577951f

#define alpha 0.99f
//functions
extern void vMPU6050Init();
extern uint8_t mpu6050_testConnection();
extern void mpu6050_setSleepDisabled();
extern void mpu6050_setSleepEnabled();
extern int8_t mpu6050_readBytes(uint8_t regAddr, uint8_t length, uint8_t *data);
extern int8_t mpu6050_readByte(uint8_t regAddr, uint8_t *data);
extern void mpu6050_writeBytes(uint8_t regAddr, uint8_t length, uint8_t* data);
extern void mpu6050_writeByte(uint8_t regAddr, uint8_t data);
extern int8_t mpu6050_readBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
extern int8_t mpu6050_readBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data);
extern void mpu6050_writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
extern void mpu6050_writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data);

//Complementary Filter
//union raw data
typedef union ACCEL_GYRO_UNION
{
	struct
	{
		uint8_t x_accel_h;
		uint8_t x_accel_l;
		uint8_t y_accel_h;
		uint8_t y_accel_l;
		uint8_t z_accel_h;
		uint8_t z_accel_l;
		uint8_t t_h;
		uint8_t t_l;
		uint8_t x_gyro_h;
		uint8_t x_gyro_l;
		uint8_t y_gyro_h;
		uint8_t y_gyro_l;
		uint8_t z_gyro_h;
		uint8_t z_gyro_l;
	} reg;
	struct
	{
		int x_accel;
		int y_accel;
		int z_accel;
		int temperature;
		int x_gyro;
		int y_gyro;
		int z_gyro;
	} value;
}accel_t_gyro_union;

extern void read_gyro_accel_vals(uint8_t* accel_t_gyro_ptr);
void calibrate_sensors();
void AZ(unsigned long t_now,float *angle_x,float *angle_y,float *angle_z);

#endif
