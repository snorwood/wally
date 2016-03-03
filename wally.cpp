/*
  wally.cpp - Library for controlling wally the wall climbing robot.
  Created by Team Wall-R-Us, March 1, 2016.
*/

#include "wally.h"

/*
Function: 		Wally
Description: 	Constructor, initializes components
*/
Wally::Wally() {
	/* Acclerometer */
	mma = new Adafruit_MMA8451();
	mma->begin();
  	mma->setRange(MMA8451_RANGE_4_G);

	/* Ultrasonic */
	us_f = new NewPing(US_F_T, US_F_E, US_MAX_DISTANCE);
	us_r = new NewPing(US_R_T, US_R_E, US_MAX_DISTANCE);

	/* Button */
	pinMode(BUTTON, INPUT);
	pinMode(BUTTON_REF, OUTPUT);
	digitalWrite(BUTTON_REF, HIGH);
}

/********** ACCELEROMETER ***********/

/*
Function: 		calibrateAccelerometer
Description: 	Calibrates the accelerometer offset based on current reading
*/
void Wally::calibrateAccelerometer(){
	accelerometerOffset.x = 0;
	accelerometerOffset.y = 0;
	accelerometerOffset.z = 0;
}

/*
Function: 		readAccelerometer
Description: 	Reads the current values from the accelerometer
Returns:		XYZ struct of current accelerometer data
*/
XYZ Wally::readAccelerometer() {
	XYZ accData;
	
	mma->read();
	accData.x = convertAccelerometer(mma->x);
	accData.y = convertAccelerometer(mma->y);
	accData.z = convertAccelerometer(mma->z); 

	return accData;
}

/********** ULTRASONIC ***********/

/*
Function:		readUltrasonic
Description:	Reads the current value from an ultrasonic
Parameters:
	address:	Selects ultrasonic sensor -> 0=FRONT, 1=REAR
Returns:		Float of selected ultrasonic distance measurement	
*/
float Wally::readUltrasonic(int address) {
	if (address == 0)
		return us_f->ping() / float(US_CONVERT);

	return us_r->ping() / float(US_CONVERT);
}

/********** BUTTON ***********/

/*
Function: 		waitButton
Description:	Waits until the button is pressed
*/
void Wally::waitButton() {
	while (!digitalRead(BUTTON));
}

/********** MOTORS ***********/

/*
Function:		setMotors
Description:	Sets the speed to drive all motors at
Parameters:
	left_speed:	float speed 0%-100% to set left motors
	right_speed:float speed 0%-100% to set right motors
*/
void Wally::setMotors(float left_speed, float right_speed){
	setMotor(m_lf, left_speed);
	setMotor(m_lr, left_speed);
	setMotor(m_rf, right_speed);
	setMotor(m_rr, right_speed);
}

/**************************************
				PRIVATE
**************************************/

/********** ACCELEROMETER ***********/

/*
Function:		convertAccelerometer
Description:	Converts accelerometer ADC count to m/s^2
Parameters:
	count:		int count is a 14-bit value
Returns:		Acceleration in m/s^2
*/
float Wally::convertAccelerometer(int count) {
  return count * ADC_CONVERT;
}

/********** MOTOR ***********/

/*
Function:		setMotor
Description:	Interface for controlling the speed of a single motor
Parameters:
	m:			Motor to control
	speed:		Speed 0%-100% to set the motor to
*/
void Wally::setMotor(Motor m, float speed) {
	m.speed = speed;
	speed = speed * SPEED_CONVERT;

	if (speed > 0) {
		digitalWrite(m.p, speed);
		digitalWrite(m.n, 0);
	} else {
		digitalWrite(m.p, 0);
		digitalWrite(m.n, speed);
	}
}
