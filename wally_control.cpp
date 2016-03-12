/*
  wally_control.cpp - Control loop library for wally the robot.
  Created by Team Wall-R-Us, March 5, 2016.
*/

#include "wally_control.h"

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
	t1 = micros();
	theta = wally->getTheta(wally->readAccelerometer());
	err1 = 0 - theta;
}

/*
Function:		verticalControl
Description:	Run in your loop to give robot vertical control
*/
void WallyControl::verticalControl(float time_us, float speed_r, float theta_r) {
	/* Collect Loop Variables */
	theta = wally->getTheta(wally->readAccelerometer());
	err2 = theta_r - theta;
	derr_dt = (err2-err1)/(time_us-t1);

	/* Calculate Motor Inputs */
	float u = kp * err2 + kd * derr_dt;
	if (u > INPUT_LIMIT) {
		u = INPUT_LIMIT;
	} else if (u < -INPUT_LIMIT) {
		u = -INPUT_LIMIT;
	}

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
	t1 = time_us;
	err1 = err2;
}

/*
Function:		horizontalControl
Description:	Run in your loop to give robot horizontal control
*/
void WallyControl::horizontalControl(float time_us, float x_r) {
	float x = wally->readUltrasonic(1);
	float u = hp * (x_r + x);
	if (u > MOTOR_LIMIT) {
		u = MOTOR_LIMIT;
	}
	
	wally->setMotors(u, u);
	t1 = time_us;
}