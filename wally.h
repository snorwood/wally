/*
  wally.h - Library for controlling wally the wall climbing robot.
  Created by Team Wall-R-Us, March 1, 2016.
*/
#ifndef wally_h
#define wally_h

#include <Arduino.h>
#include <Adafruit_MMA8451.h>
#include <NewPing.h>

/* Defines Motor Constants */
const float SPEED_CONVERT = 255 / 100.0;
const int LF_P = 3;		// Front Left High
const int LF_N = 2;		// Front Left Low
const int LR_P = 7;		// Left Rear High
const int LR_N = 6;		// Left Rear Low
const int RF_P = 4;		// Right Front High
const int RF_N = 5;		// Right Front Low
const int RR_P = 8;		// Right Rear High
const int RR_N = 9;		// Right Rear Low

/* Define Ultrasonic Constants */
const int US_F_E = 35;	// Front Echo
const int US_F_T = 33;	// Front Trigger
const int US_R_E = 32;	// Rear Echo
const int US_R_T = 34; 	// Rear Trigger

const int US_MAX_DISTANCE = 200;	// Maximum detection range
const int US_CONVERT = 58;			// Convert from us to cm

/* Define Accelerometer Constants */
const float ADC_CONVERT = 8 * 9.81 / 16384;	// Convert from 14-bit count to m/s^2
const float GRAVITY_THRESHOLD = 9.00;

/* Define Button Constants */
const int BUTTON = 40;		// Button input pin
const int BUTTON_REF = 36;	// Button high reference pin

/* Define IR Constants */
const int IR = 31;

/*
Struct: XYZ
Description: Stores three dimensional float data
*/
struct XYZ {
	float x;
	float y;
	float z;
};

/*
Struct: Motor
Description: Contains parameters for a motor
*/
struct Motor {
	int p;
	int n;
	float speed;
};

enum Orientation {
	up,
	down,
	flat
};

/*
Class: Wally
Description: Controls an instance of the wally robot
*/
class Wally {
public:
	Wally();

	/* Ultrasonic */
	float readUltrasonic(int address);

	/* Accelerometer */
	void calibrateAccelerometer();
	XYZ readAccelerometer();
	static Orientation getOrientation(XYZ acc);
	static float getTheta(XYZ acc);

	/* Button */
	int readButton();
	void waitButton();

	/* Motors */
	void setMotors(float left_speed, float right_speed);
	void setMotorsFR(float front_speed, float rear_speed);
	void stop();

	/* IR */
	int readIR();

private:
	/* Acclerometer */
	Adafruit_MMA8451* mma;
	XYZ accelerometerOffset;
	float convertAccelerometer(int count);

	/* Ultrasonic */
	NewPing* us_f;
	NewPing* us_r;

	/* Motors */
	static void setMotor(Motor m, float speed);
	Motor m_lf = {LF_P, LF_N, 0};	// Left Front Motor
	Motor m_lr = {LR_P, LR_N, 0};	// Left Rear Motor
	Motor m_rf = {RF_P, RF_N, 0};	// Right Front Motor
	Motor m_rr = {RR_P, RR_N, 0};	// Right Rear Motor
};

#endif