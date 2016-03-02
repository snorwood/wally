/*
  wally.h - Library for controlling wally the wall climbing robot.
  Created by Team Group 2, March 1, 2016.
*/
#ifndef wally_h
#define wally_h

#include <Adafruit_MMA8451.h>
#include <NewPing.h>

/* Defines Motor Constants */
const int LF_L = 2;
const int LF_H = 3;
const int RF_H = 4;
const int RF_L = 5;
const int LR_L = 6;
const int LR_H = 7;
const int RR_H = 8;
const int RR_L = 9;

/* Define Ultrasonic Constants */
const int US_F_E = 1;
const int US_F_T = 2;
const int US_R_E = 3;
const int US_R_T = 4; 
const int US_MAX_DISTANCE = 200;
const int US_CONVERT = 58;

/* Define Accelerometer Constants */
const float ADC_CONVERT = 8 * 9.81 / 16384;

/* Define Button Constants */
const int BUTTON = 40;
const int BUTTON_REF = 36;

// struct xyz -> Base of XYZ
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

class Wally {
public:
	Wally();

	/* Ultrasonic */
	float readUltrasonic(int address);

	/* Accelerometer */
	void calibrateAccelerometer();
	XYZ readAccelerometer();

private:
	Adafruit_MMA8451* mma;
	XYZ accelerometerOffset;
	float convertAccelerometer(int count);
	NewPing* us_f;
	NewPing* us_r;
};

#endif