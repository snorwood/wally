/*
  wally.cpp - Library for controlling wally the wall climbing robot.
  Created by Team Wall-R-Us, March 1, 2016.
*/

#include "wally.h"
#include <math.h>

/**************************************
				PUBLIC
**************************************/

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

	/* IR */
	pinMode(IR, INPUT);

	/* Motors */
	pinMode(LF_P, OUTPUT);		// Front Left High
	pinMode(LF_N, OUTPUT);		// Front Left Low
	pinMode(LR_P, OUTPUT);		// Left Rear High
	pinMode(LR_N, OUTPUT);		// Left Rear Low
	pinMode(RF_P, OUTPUT);		// Right Front High
	pinMode(RF_N, OUTPUT);		// Right Front Low
	pinMode(RR_P, OUTPUT);		// Right Rear High
	pinMode(RR_N, OUTPUT);		// Right Rear Low
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

/*
Function: 		getOrientation
Description: 	Determines the orientation of the robot from acceleration
Parameters:
	acc:		XYZ acceleration of the robot
Returns:		Orientation
*/
Orientation Wally::getOrientation(XYZ acc) {
	if(acc.y < -GRAVITY_THRESHOLD)
		return DOWN;

	if (acc.y > GRAVITY_THRESHOLD)
		return UP;

	return FLAT;
}

/*
Function: 		getTheta
Description: 	Calculatest the rotation of the robot on the wall
Parameters:
	acc:		XYZ acceleration of the robot
Returns:		float theta
*/
float Wally::getTheta(XYZ acc) {
	return atan2(acc.x, -acc.y);
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
	float us = 0;

	if (address == 0)
		us = us_f->ping() / float(US_CONVERT);
	else
		us = us_r->ping() / float(US_CONVERT);

	if (us < US_MIN_DISTANCE)
		return US_MIN_DISTANCE;
	
	return us;
}

/********** BUTTON ***********/

/*
Function: 		waitButton
Description:	Waits until the button is pressed
*/
void Wally::waitButton() {
	while (!digitalRead(BUTTON));
}

/*
Function: 		readButton
Description:	Returns the status of the button
*/
int Wally::readButton() {
	return digitalRead(BUTTON);
}

/********** MOTORS ***********/

/*
Function:		setMotors
Description:	Sets the speed to drive all motors separated by left and right
Parameters:
	left_speed:	float speed 0%-100% to set left motors
	right_speed:float speed 0%-100% to set right motors
*/
void Wally::setMotors(float left_speed, float right_speed) {
	setMotor(m_rr, right_speed);
	setMotor(m_lr, left_speed);
	setMotor(m_lf, left_speed);
	setMotor(m_rf, right_speed);
}

/*
Function:		setMotorsFR
Description:	Sets the speed to drive all motors separated by front and rear
Parameters:
	front_speed:	float speed 0%-100% to set front motors
	rear_speed:float speed 0%-100% to set rear motors
*/
void Wally::setMotorsFR(float front_speed, float rear_speed) {
	setMotor(m_rr, rear_speed);
	setMotor(m_lr, rear_speed);
	setMotor(m_lf, front_speed);
	setMotor(m_rf, front_speed);
}

/*
Function:		stop
Description:	Stops the robot
*/
void Wally::stop() {
	setMotors(0,0);
}

/*
Function:		readIR
Description:	Reads the IR sensor
Returns:		int status of IR sensor on=1, off=0 
*/
int Wally::readIR(){
	int sum = 0;
	for (int i = 0; i < 5; i++)
		sum += !digitalRead(IR);
	
	return sum >= 3;
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
		analogWrite(m.p, speed);
		analogWrite(m.n, 0);
	} else {
		analogWrite(m.p, 0);
		analogWrite(m.n, speed);
	}
}