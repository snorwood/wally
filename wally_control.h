/*
  wally_control.h - Control loop library for wally the robot.
  Created by Team Wall-R-Us, March 5, 2016.
*/

#ifndef wally_control_h
#define wally_control_h

#include "wally.h"

const float MOTOR_LIMIT = 100;
const float INPUT_LIMIT = 50;

float defaultThetaR(float t);

/*
Class: WallyControl
Description: Provides high level control loops and functions for Wally
*/
class WallyControl {
public:
	/* Contstructor */
	WallyControl(Wally* wally);

	/* Vertical Control */
	void begin();
	void verticalControl(float time_us, float speed, float theta_r);

	/* Horizontal Control */
	void horizontalControl(float time_us, float x_r);

private:
	/* Robot */
	Wally* wally;	// Robot instance

	/* Loop Parameters */
	float kp = 1;
	float kd = 100;
	float hp = 2;

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