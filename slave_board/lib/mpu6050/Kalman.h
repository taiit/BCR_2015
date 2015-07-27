/*
 * IncFile1.h
 *
 * Created: 6/10/2015 7:59:55 PM
 *  Author: Vo Huu Tai
 */ 


#ifndef KALMAN_H_
#define KALMAN_H_


struct KALMAN_DATA{
	 /* Kalman filter variables */
	 float Q_angle; // Process noise variance for the accelerometer
	 float Q_bias; // Process noise variance for the gyro bias
	 float R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

	 float angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
	 float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
	 float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

	 float P[2][2]; // Error covariance matrix - This is a 2x2 matrix
	 
	 unsigned long t_pre;
	};
void Kalman(struct KALMAN_DATA *sKalmanData) {
	/* We will set the variables like so, these can also be tuned by the user */
	sKalmanData->Q_angle = 0.001f;
	sKalmanData->Q_bias = 0.003f;
	sKalmanData->R_measure = 0.03f;

	sKalmanData->angle = 0.0f; // Reset the angle
	sKalmanData->bias = 0.0f; // Reset bias

	sKalmanData->P[0][0] = 0.0f; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
	sKalmanData->P[0][1] = 0.0f;
	sKalmanData->P[1][0] = 0.0f;
	sKalmanData->P[1][1] = 0.0f;
	
	sKalmanData->t_pre = 0;
};
// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
float getAngle(struct KALMAN_DATA *sKalmanData,float newAngle, float newRate,float dt){
	 //float dt = ;t_now - sKalmanData->t_pre;
	 // Discrete Kalman filter time update equations - Time Update ("Predict")
	 // Update xhat - Project the state ahead
	 /* Step 1 */
	 sKalmanData->rate = newRate - sKalmanData->bias;
	 sKalmanData->angle += dt * sKalmanData->rate;

	 // Update estimation error covariance - Project the error covariance ahead
	 /* Step 2 */
	 sKalmanData->P[0][0] += dt * (dt*sKalmanData->P[1][1] - sKalmanData->P[0][1] - sKalmanData->P[1][0] + sKalmanData->Q_angle);
	 sKalmanData->P[0][1] -= dt * sKalmanData->P[1][1];
	 sKalmanData->P[1][0] -= dt * sKalmanData->P[1][1];
	 sKalmanData->P[1][1] += sKalmanData->Q_bias * dt;

	 // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
	 // Calculate Kalman gain - Compute the Kalman gain
	 /* Step 4 */
	 float S = sKalmanData->P[0][0] + sKalmanData->R_measure; // Estimate error
	 /* Step 5 */
	 float K[2]; // Kalman gain - This is a 2x1 vector
	 K[0] = sKalmanData->P[0][0] / S;
	 K[1] = sKalmanData->P[1][0] / S;

	 // Calculate angle and bias - Update estimate with measurement zk (newAngle)
	 /* Step 3 */
	 float y = newAngle - sKalmanData->angle; // Angle difference
	 /* Step 6 */
	 sKalmanData->angle += K[0] * y;
	 sKalmanData->bias += K[1] * y;

	 // Calculate estimation error covariance - Update the error covariance
	 /* Step 7 */
	 float P00_temp = sKalmanData->P[0][0];
	 float P01_temp = sKalmanData->P[0][1];

	 sKalmanData->P[0][0] -= K[0] * P00_temp;
	 sKalmanData->P[0][1] -= K[0] * P01_temp;
	 sKalmanData->P[1][0] -= K[1] * P00_temp;
	 sKalmanData->P[1][1] -= K[1] * P01_temp;

	 return sKalmanData->angle;
}
void setAngle(struct KALMAN_DATA *sKalmanData,float _angle) 
{
	 sKalmanData->angle = _angle; 
}
float getRate(struct KALMAN_DATA *data) { return data->rate; } // Return the unbiased rate
#endif /* INCFILE1_H_ */