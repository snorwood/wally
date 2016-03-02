/*
  wally.h - Library for controlling wally the wall climbing robot.
  Created by Team Group 2, March 1, 2016.
*/
#ifndef wally_h
#define wally_h

// Defines Motor Pins
const int LF_L = 2;
const int LF_H = 3;
const int RF_H = 4;
const int RF_L = 5;
const int LR_L = 6;
const int LR_H = 7;
const int RR_H = 8;
const int RR_L = 9;

const float ADC_CONVERT = 8 * 9.81 / 16384;

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

float adcConvert(int count);

float readUltrasonic(int address);

void calibrateAccelerometer();
XYZ readAccelerometer();

#endif