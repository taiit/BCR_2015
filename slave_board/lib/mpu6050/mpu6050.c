/*
MPU6050 lib 0x02

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#include "mpu6050.h"
#include "../inc/common.h"

//path to i2c fleury lib
#include MPU6050_I2CFLEURYPATH

volatile uint8_t g_uibuffer[14];

/*
 * read bytes from chip register
 */
int8_t mpu6050_readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
	uint8_t i = 0;
	int8_t count = 0;
	if(length > 0) {
		//request register
		i2c_start(MPU6050_ADDR | I2C_WRITE);
		i2c_write(regAddr);
		_delay_us(10);
		//read data
		i2c_start(MPU6050_ADDR | I2C_READ);
		for(i=0; i<length; i++) {
			count++;
			if(i==length-1)
				data[i] = i2c_readNak();
			else
				data[i] = i2c_readAck();
		}
		i2c_stop();
	}
	return count;
}

/*
 * read 1 byte from chip register
 */
int8_t mpu6050_readByte(uint8_t regAddr, uint8_t *data) {
    return mpu6050_readBytes(regAddr, 1, data);
}

/*
 * write bytes to chip register
 */
void mpu6050_writeBytes(uint8_t regAddr, uint8_t length, uint8_t* data) {
	if(length > 0) {
		//write data
		i2c_start(MPU6050_ADDR | I2C_WRITE);
		i2c_write(regAddr); //reg
		for (uint8_t i = 0; i < length; i++) {
			i2c_write((uint8_t) data[i]);
		}
		i2c_stop();
	}
}

/*
 * write 1 byte to chip register
 */
void mpu6050_writeByte(uint8_t regAddr, uint8_t data) {
    return mpu6050_writeBytes(regAddr, 1, &data);
}

/*
 * read bits from chip register
 */
int8_t mpu6050_readBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    int8_t count = 0;
    if(length > 0) {
		uint8_t b;
		if ((count = mpu6050_readByte(regAddr, &b)) != 0) {
			uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
			b &= mask;
			b >>= (bitStart - length + 1);
			*data = b;
		}
    }
    return count;
}

/*
 * read 1 bit from chip register
 */
int8_t mpu6050_readBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
    uint8_t b;
    uint8_t count = mpu6050_readByte(regAddr, &b);
    *data = b & (1 << bitNum);
    return count;
}

/*
 * write bit/bits to chip register
 */
void mpu6050_writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
	if(length > 0) {
		uint8_t b = 0;
		if (mpu6050_readByte(regAddr, &b) != 0) { //get current data
			uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
			data <<= (bitStart - length + 1); // shift data into correct position
			data &= mask; // zero all non-important bits in data
			b &= ~(mask); // zero all important bits in existing byte
			b |= data; // combine data with existing byte
			mpu6050_writeByte(regAddr, b);
		}
	}
}

/*
 * write one bit to chip register
 */
void mpu6050_writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    mpu6050_readByte(regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    mpu6050_writeByte(regAddr, b);
}

/*
 * set sleep disabled
 */
void mpu6050_setSleepDisabled() {
	mpu6050_writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 0);
}

/*
 * set sleep enabled
 */
void mpu6050_setSleepEnabled() {
	mpu6050_writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 1);
}

/*
 * test connection to chip
 */
uint8_t mpu6050_testConnection() {
	uint8_t data[1];
	mpu6050_readByte(MPU6050_RA_WHO_AM_I,data);
	return data[0];
	////if(g_uibuffer[0] == 0x34)
	//if(g_uibuffer[0] == 0x68)
		//return 1;
	//else
		//return 0;
}

/*
 * initialize the accel and gyro
 */
void vMPU6050Init()
{
	//init i2c
	i2c_init();
	_delay_us(10);

	//allow mpu6050 chip clocks to start up
	_delay_ms(100);

	//set sleep disabled
	mpu6050_setSleepDisabled();
	//wake up delay needed sleep disabled
	_delay_ms(10);

	//set clock source
	//  it is highly recommended that the device be configured to use one of the gyroscopes (or an external clock source)
	//  as the clock reference for improved stability
	mpu6050_writeBits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, MPU6050_CLOCK_PLL_XGYRO);
	//set DLPF bandwidth to 42Hz
	mpu6050_writeBits(MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, MPU6050_DLPF_BW_42);
    //set sampe rate
	mpu6050_writeByte(MPU6050_RA_SMPLRT_DIV, 4); //1khz / (1 + 4) = 200Hz
	//set gyro range
	mpu6050_writeBits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, MPU6050_GYRO_FS);
	//set accel range
	mpu6050_writeBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, MPU6050_ACCEL_FS);

}

#if 0
/*
 try code :D :D
*/
void read_gyro_accel_vals(uint8_t* accel_t_gyro_ptr) {
	// Read the raw values.
	// Read 14 bytes at once,
	// containing acceleration, temperature and gyro.
	
	accel_t_gyro_union* accel_t_gyro = (accel_t_gyro_union *) accel_t_gyro_ptr;
	
	mpu6050_readBytes(MPU6050_RA_ACCEL_XOUT_H, sizeof(*accel_t_gyro), (uint8_t *) accel_t_gyro);

	// Swap all high and low bytes.
	uint8_t swap;
	#define SWAP(x,y) swap = x; x = y; y = swap

	SWAP ((*accel_t_gyro).reg.x_accel_h, (*accel_t_gyro).reg.x_accel_l);
	SWAP ((*accel_t_gyro).reg.y_accel_h, (*accel_t_gyro).reg.y_accel_l);
	SWAP ((*accel_t_gyro).reg.z_accel_h, (*accel_t_gyro).reg.z_accel_l);
	SWAP ((*accel_t_gyro).reg.t_h, (*accel_t_gyro).reg.t_l);
	SWAP ((*accel_t_gyro).reg.x_gyro_h, (*accel_t_gyro).reg.x_gyro_l);
	SWAP ((*accel_t_gyro).reg.y_gyro_h, (*accel_t_gyro).reg.y_gyro_l);
	SWAP ((*accel_t_gyro).reg.z_gyro_h, (*accel_t_gyro).reg.z_gyro_l);
}

//  Use the following global variables and access functions
//  to calibrate the acceleration sensor

float    gf_base_x_accel;
float    gf_base_y_accel;
float    gf_base_z_accel;

float    gf_base_x_gyro;
float    gf_base_y_gyro;
float    gf_base_z_gyro;
// The sensor should be motionless on a horizontal surface
//  while calibration is happening
void calibrate_sensors() 
{
	uint8_t               num_readings = 20;
	float                 x_accel = 0;
	float                 y_accel = 0;
	float                 z_accel = 0;
	float                 x_gyro = 0;
	float                 y_gyro = 0;
	float                 z_gyro = 0;
	accel_t_gyro_union    accel_t_gyro;
	
	//Serial.println("Starting Calibration");

	// Discard the first set of values read from the IMU
	read_gyro_accel_vals((uint8_t *) &accel_t_gyro);
	
	// Read and average the raw values from the IMU
	for (uint8_t i = 0; i < num_readings; i++) {
		read_gyro_accel_vals((uint8_t *) &accel_t_gyro);
		x_accel += accel_t_gyro.value.x_accel;
		y_accel += accel_t_gyro.value.y_accel;
		z_accel += accel_t_gyro.value.z_accel;
		x_gyro += accel_t_gyro.value.x_gyro;
		y_gyro += accel_t_gyro.value.y_gyro;
		z_gyro += accel_t_gyro.value.z_gyro;
		_delay_ms(100);
		//LED_BUG_BLINK;
	}
	x_accel /= num_readings;
	y_accel /= num_readings;
	z_accel /= num_readings;
	x_gyro /= num_readings;
	y_gyro /= num_readings;
	z_gyro /= num_readings;
	
	// Store the raw calibration values globally
	gf_base_x_accel = x_accel;
	gf_base_y_accel = y_accel;
	gf_base_z_accel = z_accel;
	gf_base_x_gyro = x_gyro;
	gf_base_y_gyro = y_gyro;
	gf_base_z_gyro = z_gyro;
	//Serial.println("Finishing Calibration");
}

// Use the following global variables and access functions to help store the overall
// rotation angle of the sensor
unsigned long last_read_time;
float         gf_flast_x_angle;  // These are the filtered angles
float         gf_last_y_angle;
float         gf_last_z_angle;
float         gf_last_gyro_x_angle;  // Store the gyro angles to compare drift
float         gf_last_gyro_y_angle;
float         gf_last_gyro_z_angle;

void set_last_read_angle_data(unsigned long time,float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) {
	
	last_read_time = time;
	gf_flast_x_angle = x;
	gf_last_y_angle = y;
	gf_last_z_angle = z;
	gf_last_gyro_x_angle = x_gyro;
	gf_last_gyro_y_angle = y_gyro;
	gf_last_gyro_z_angle = z_gyro;
}
inline unsigned long get_last_time() {return last_read_time;}
inline float get_last_x_angle() {return gf_flast_x_angle;}
inline float get_last_y_angle() {return gf_last_y_angle;}
inline float get_last_z_angle() {return gf_last_z_angle;}
inline float get_last_gyro_x_angle() {return gf_last_gyro_x_angle;}
inline float get_last_gyro_y_angle() {return gf_last_gyro_y_angle;}
inline float get_last_gyro_z_angle() {return gf_last_gyro_z_angle;}
	
void AZ(unsigned long t_now,float *angle_x,float *angle_y,float *angle_z)
{	
	accel_t_gyro_union accel_t_gyro;
	// Read the raw values.
	read_gyro_accel_vals((uint8_t*) &accel_t_gyro);
	
	// Convert gyro values to degrees/sec
	float FS_SEL = 131;
	float gyro_x = (accel_t_gyro.value.x_gyro - gf_base_x_gyro)/FS_SEL;
	float gyro_y = (accel_t_gyro.value.y_gyro - gf_base_y_gyro)/FS_SEL;
	float gyro_z = (accel_t_gyro.value.z_gyro - gf_base_z_gyro)/FS_SEL;
	// Get raw acceleration values
	//float G_CONVERT = 16384;
	float accel_x = accel_t_gyro.value.x_accel;
	float accel_y = accel_t_gyro.value.y_accel;
	float accel_z = accel_t_gyro.value.z_accel;
	
	 // Get angle values from accelerometer
	 //  float accel_vector_length = sqrt(pow(accel_x,2) + pow(accel_y,2) + pow(accel_z,2));
	 float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
	 float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;

	 //float accel_angle_z = 0; dont use
	 // Compute the (filtered) gyro angles
	 float DT =(t_now - get_last_time())/1000.0;
	 float gyro_angle_x = gyro_x*DT + get_last_x_angle();
	 float gyro_angle_y = gyro_y*DT + get_last_y_angle();
	 float gyro_angle_z = gyro_z*DT + get_last_z_angle();
	 
	 // Compute the drifting gyro angles
	 float unfiltered_gyro_angle_x = gyro_x*DT + get_last_gyro_x_angle();
	 float unfiltered_gyro_angle_y = gyro_y*DT + get_last_gyro_y_angle();
	 float unfiltered_gyro_angle_z = gyro_z*DT + get_last_gyro_z_angle();
	 
	 // Apply the complementary filter to figure out the change in angle - choice of alpha is
	 // estimated now.  Alpha depends on the sampling rate...
	 //float alpha = 0.90;
	 *angle_x = alpha*gyro_angle_x + (1.0 - alpha)*accel_angle_x;
	 *angle_y = alpha*gyro_angle_y + (1.0 - alpha)*accel_angle_y;
	 *angle_z = gyro_angle_z;  //Accelerometer doesn't give z-angle
	 
	 // Update the saved data with the latest values
	 set_last_read_angle_data(t_now,*angle_x, *angle_y, *angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
}
#endif
