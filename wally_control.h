/*
  wally_control.h - Control loop library for wally the robot.
  Created by Team Wall-R-Us, March 5, 2016.
*/

#ifndef wally_control_h
#define wally_control_h

#include "wally.h"

const float kp = 1;
const float kd = 1;
const float MOTOR_LIMIT = 50;

float defaultThetaR(float t);

class WallyControl {
public:
	WallyControl(Wally* wally);
	void begin();
	void begin(float(*theta_r)(float));
	void verticalControl();
	void horizontalControl();

private:
	/* Robot */
	Wally* wally;	// Robot instance

	/* Time */
	int t1 = 0;		// Time previous
	int t2 = 0;		// Time new
	
	/* Motor Inputs */
	float ul;	// Right motor input
	float ur;	// Left motor input

	/* Theta */
	float(*theta_r)(float); // Command theta_r function
	float theta= 0;			// Current theta
	float err1 = 0;			// Theta error previous
	float err2 = 0;			// Theta error new
	float derr_dt = 0;		// delta error / dt
};

#endif