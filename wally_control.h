/*
  wally_control.h - Control loop library for wally the robot.
  Created by Team Wall-R-Us, March 5, 2016.
*/

#ifndef wally_control_h
#define wally_control_h

#include "wally.h"

const float MOTOR_LIMIT = 100;
const float INPUT_LIMIT = 50;
const int MOTOR_CORRECT = 3;

float defaultThetaR(float t);

class WallyControl {
public:
	WallyControl(Wally* wally);
	void begin();
	void verticalControl(float time_us, float speed, float theta_r);
	void horizontalControl(float time_us, float x_r);

private:
	/* Robot */
	Wally* wally;	// Robot instance

	/* Loop Parameters */
	float kp = 500;
	float kd = 1;
	float hp = 1.8;

	/* Time */
	int t1 = 0;		// Time previous
	
	/* Motor Inputs */
	float ul;	// Right motor input
	float ur;	// Left motor input

	/* Theta */
	float theta = 0;		// Current theta
	float err1 = 0;			// Theta error previous
	float err2 = 0;			// Theta error new
	float derr_dt = 0;		// delta error / dt
};

#endif