/*
  wally_control.h - Control loop library for wally the robot.
  Created by Team Wall-R-Us, March 5, 2016.
*/
#ifndef wally_control_h
#define wally_control_h

#include "wally.h"
#include "rolling_median.h"

/* Vertical Control Constants */
const float MOTOR_LIMIT = 100;
const float INPUT_LIMIT = 80;

/* Horizontal Control Constants */
const float TURN_DELAY = 350/90.0;
const float TURN_SPEED = 30;
const float STRAIGHT_MAX = 30;
const float STRAIGHT_MIN = 20;
const float BASE_CLOSE = 20;
const float BASE_DIFFERENTIAL_L = 40;
const float BASE_DIFFERENTIAL_R = 30;
	
enum DIR {
	LEFT,
	RIGHT,
	FRONT,
	NONE
};

/*
Class: WallyControl
Description: Provides high level control loops and functions for Wally
*/
class WallyControl {
public:
	/* Constructor */
	WallyControl(Wally* wally, float vp=500, float vd=1, float hd=0.7, int sampleSize=3);

	/* Loop Update */
	void beginVertical();
	void beginHorizontal();
	void update();
	
	/* Accessor Functions */
	XYZ readAccelerometer();
	int readIR();
	float readUltrasonic(int address);

	/* Vertical Control */
	void verticalControl(float speed, float theta_r);

	/* Horizontal Control */
	void horizontalControl();
	void turn(DIR direction, float degrees);
	DIR findBase();

private:
	/* Robot */
	Wally* wally;	// Robot instance

	/* Loop Parameters */
	float vp = 500;
	float vd = 1;
	float hd = 0.7;

	/* Time */
	int t1 = 0;		// Time previous
	int t2 = 0;		// Time current
	int sampleSize = 3;

	/* Motor Inputs */
	float ul;	// Right motor input
	float ur;	// Left motor input

	/* Theta */
	float theta = 0;		// Current theta
	float err1 = 0;			// Theta error previous
	float err2 = 0;			// Theta error new
	float derr_dt = 0;		// delta error / dt

	/* Horizontal Control Loop Variables */
	float us_left_1;
	float us_left_2;
	float d_left;
	float us_right_1;
	float us_right_2;
	float d_right;


	/* Rolling Medians */
	RollingMedian<float>* rm_acc_x;
	RollingMedian<float>* rm_acc_y;
	RollingMedian<float>* rm_acc_z;
	RollingMedian<float>* rm_us_f;
	RollingMedian<float>* rm_us_r;
	RollingMedian<float>* rm_us_l;
	RollingMedian<int>*	 rm_ir;
};

#endif