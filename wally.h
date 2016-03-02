/*
  wally.h - Library for controlling wally the wall climbing robot.
  Created by Team Group 2, March 1, 2016.
*/
#ifndef wally_h
#define wally_h

#include <Arduino.h>
#include <Adafruit_MMA8451.h>
#include <NewPing.h>

/* Defines Motor Constants */
const int LF_L = 2;		// Front Left Low
const int LF_H = 3;		// Front Left High
const int RF_H = 4;		// Right Front High
const int RF_L = 5;		// Right Front Low
const int LR_L = 6;		// Left Rear Low
const int LR_H = 7;		// Left Rear High
const int RR_H = 8;		// Right Rear High
const int RR_L = 9;		// Right Rear Low

/* Define Ultrasonic Constants */
const int US_F_E = 1;	// Front Echo
const int US_F_T = 2;	// Front Trigger
const int US_R_E = 3;	// Rear Echo
const int US_R_T = 4; 	// Rear Trigger

const int US_MAX_DISTANCE = 200;	// Maximum detection range
const int US_CONVERT = 58;			// Convert from us to cm

/* Define Accelerometer Constants */
const float ADC_CONVERT = 8 * 9.81 / 16384;	// Convert from 14-bit count to m/s^2

/* Define Button Constants */
const int BUTTON = 40;		// Button input pin
const int BUTTON_REF = 36;	// Button high reference pin

/* struct xyz: Base of XYZ */
struct xyz {
	float x;
	float y;
	float z;
};

/*
	Struct: XYZ
	Description: Stores three dimensional float data
*/
typedef struct xyz XYZ;

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

	/* Button */
	void waitButton();

private:
	/* Acclerometer */
	Adafruit_MMA8451* mma;
	XYZ accelerometerOffset;
	float convertAccelerometer(int count);

	/* Ultrasonic */
	NewPing* us_f;
	NewPing* us_r;
};

#endif