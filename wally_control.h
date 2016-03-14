/*
  wally_control.h - Control loop library for wally the robot.
  Created by Team Wall-R-Us, March 5, 2016.
*/

#ifndef wally_control_h
#define wally_control_h

#include "wally.h"
#include "rolling_median.h"

const float MOTOR_LIMIT = 100;
const float INPUT_LIMIT = 50;
const int MOTOR_CORRECT = 3;

float defaultThetaR(float t);

/*
Class: WallyControl
Description: Provides high level control loops and functions for Wally
*/
class WallyControl {
public:
	/* Constructor */
	WallyControl(Wally* wally);

	/* Loop Update */
	void begin();
	void update();
	
	/* Accessor Functions */
	XYZ readAccelerometer();
	int readIR();
	float readUltrasonic(int address);

	/* Vertical Control */
	void verticalControl(float speed, float theta_r);

	/* Horizontal Control */
	void horizontalControl(float x_r);

private:
	/* Robot */
	Wally* wally;	// Robot instance

	/* Loop Parameters */
	float kp = 500;
	float kd = 1;
	float hp = 1.8;

	/* Time */
	int t1 = 0;		// Time previous
	int t2 = 0;		// Time current
	int sampleSize = 10;

	/* Motor Inputs */
	float ul;	// Right motor input
	float ur;	// Left motor input

	/* Theta */
	float theta = 0;		// Current theta
	float err1 = 0;			// Theta error previous
	float err2 = 0;			// Theta error new
	float derr_dt = 0;		// delta error / dt

	/* Rolling Medians */
	RollingMedian<float> rm_acc_x;
	RollingMedian<float> rm_acc_y;
	RollingMedian<float> rm_acc_z;
	RollingMedian<float> rm_us_f;
	RollingMedian<float> rm_us_r;
	RollingMedian<float> rm_us_l;
	RollingMedian<int>	 rm_ir;
};

#endif