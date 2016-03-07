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
Parameters:
	theta_r:	Default theta_r(t) = 0
*/
void WallyControl::begin() {
	begin(defaultThetaR);
}

/*
Function:		begin
Description:	Initial conditions and parameters of control loop
Parameters:
	theta_r:	Commanded angle theta_r(t)
*/
void WallyControl::begin(float(*theta_r)(float)) {
	this->theta_r = theta_r;
	t1 = micros();
	theta = wally->getTheta(wally->readAccelerometer());
	err1 = theta_r(0) - theta;
}

/*
Function:		verticalControl
Description:	Run in your loop to give robot vertical control
*/
void WallyControl::verticalControl() {
	/* Collect Loop Variables */
	t2 = micros();
	theta = wally->getTheta(wally->readAccelerometer());
	err2 = theta_r(t2) - theta;
	derr_dt = (err2-err1)/(t2-t1);

	/* Calculate Motor Inputs */
	float u = kp * err2 + kd * err_dt;
	float loop_speed = speed;
	if (abs(u) > INPUT_LIMIT) {
		if (u > 0) {
			u = INPUT_LIMIT;
		} else {
			u = -INPUT_LIMIT;
		}
	}

	if loop_speed + u / 2.0 > MOTOR_LIMIT {
		loop_speed = MOTOR_LIMIT - u / 2.0;
	} else if loop_speed - u / 2.0 < -MOTOR_LIMIT {
		loop_speed = -MOTOR_LIMIT + u / 2.0;
	}
	ur = loop_speed + u / 2.0;
	ul = loop_speed - u / 2.0;
	wally->setMotors(ul, ur);

	/* Update Stored Values */
	t1 = t2;
	err1 = err2;
}

/*
Function:		defaultThetaR
Description:	The default function for theta_r(t)
*/
float defaultThetaR(float t) {
	return 0;
}