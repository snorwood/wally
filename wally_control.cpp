/*
  wally_control.cpp - Control loop library for wally the robot.
  Created by Team Wall-R-Us, March 5, 2016.
*/

#include "wally_control.h"
#include "rolling_median.h"

/*
Function:		WallyControl
Description:	Initializes control loop
*/
WallyControl::WallyControl(Wally* wally) {
	this->wally = wally;
}

/*
Function:		begin
Description:	Initial conditions and parameters of control loop
*/
void WallyControl::begin() {
	/* Initialize Derivative Terms */
	t1 = micros();
	err1 = 0;

	/* Initialize Rolling Medians  */
	rm_acc_x = RollingMedian<float>(25);
	rm_acc_y = RollingMedian<float>(25);
	rm_acc_z = RollingMedian<float>(25);
	rm_us_0 = RollingMedian<float>(25);
	rm_us_1 = RollingMedian<float>(25);
	rm_ir = RollingMedian<int>(25);
}

void WallyControl::update() {
	/* Update Timer */
	t1 = t2;
	t2 = micros();

	/* Update Rolling Medians */
	XYZ acc = wally->readAccelerometer();
	rm_acc_x.insertSample(acc.x);
	rm_acc_y.insertSample(acc.y);
	rm_acc_z.insertSample(acc.z);

	rm_us_0.insertSample(wally->readUltrasonic(0));
	rm_us_1.insertSample(wally->readUltrasonic(1));

	rm_ir.insertSample(wally->readIR());
}

/*
Function: 		readAccelerometer
Description: 	Reads the median from the accelerometer
Returns:		XYZ struct of current accelerometer data
*/
XYZ WallyControl::readAccelerometer() {
	XYZ accData;

	accData.x = rm_acc_x.getMedian();
	accData.y = rm_acc_y.getMedian();
	accData.z = rm_acc_z.getMedian();

	return accData;
}

/*
Function:		readUltrasonic
Description:	Reads the median from an ultrasonic
Parameters:
	address:	Selects ultrasonic sensor -> 0=FRONT, 1=REAR
Returns:		Float of selected ultrasonic distance measurement	
*/
float WallyControl::readUltrasonic(int address) {
	if (address == 0)
		return rm_us_0.getMedian();

	return rm_us_1.getMedian();
}

/*
Function:		readIR
Description:	Reads the median IR sensor
Returns:		int status of IR sensor on=1, off=0 
*/
int WallyControl::readIR() {
	return rm_ir.getMedian();
}

/*
Function:		verticalControl
Description:	Run in your loop to give robot vertical control
*/
void WallyControl::verticalControl(float speed_r, float theta_r) {
	/* Collect Loop Variables */
	theta = wally->getTheta(readAccelerometer());
	err2 = theta_r - theta;
	derr_dt = (err2-err1)/(t2-t1);

	/* Calculate Motor Inputs */
	/* Ensure input differential is within limits */
	float u = kp * err2 + kd * derr_dt;
	if (abs(u) > INPUT_LIMIT) {
		if (u > 0) {
			u = INPUT_LIMIT;
		} else {
			u = -INPUT_LIMIT;
		}
	}

	/* Ensure output is within limits */
	u = u / 2.0;
	if (speed_r + abs(u) > MOTOR_LIMIT) {
		speed_r = MOTOR_LIMIT - abs(u);
	} else if (speed_r - abs(u) < -MOTOR_LIMIT) {
		speed_r = -MOTOR_LIMIT + abs(u);
	}

	ur = speed_r + u;
	ul = speed_r - u;
	wally->setMotors(ul, ur);

	/* Update Stored Values */
	err1 = err2;
}

/*
Function:		horizontalControl
Description:	Run in your loop to give robot horizontal control
*/

void WallyControl::horizontalControl(float time_us, float x_r) {
	/* Ensure output is within limits */
	float x = wally->readUltrasonic(1);
	float u = hp * (x_r + x);
	
	if (u > MOTOR_LIMIT) {
		u = MOTOR_LIMIT;
	}

	/* Send output to motors */
	wally->setMotors(u, u);
}