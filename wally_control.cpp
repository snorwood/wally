/*
  wally_control.cpp - Control loop library for wally the robot.
  Created by Team Wall-R-Us, March 5, 2016.
*/

#include "wally_control.h"
#include "rolling_median.h"

/*
Function:		WallyControl
Description:	Initializes control loop with parameters
Parameters:
	vp:			Vertical proportional gain
	vd:			Vertical derivative gain
	hp:			Horizontal proportional gain
	sampleSize:	Size of median filter samples
*/
WallyControl::WallyControl(Wally* wally, float vp, float vd, float hp, int sampleSize) {
	this->wally = wally;
	this->vp = vp;
	this->vd = vd;
	this->hp = hp;
	this->sampleSize = sampleSize;
}

/*
Function:		begin
Description:	Initial conditions and parameters of control loop
*/
void WallyControl::begin() {
	/* Initialize Derivative Terms */
	err1 = 0;

	/* Initialize Rolling Medians  */
	rm_acc_x = new RollingMedian<float>(sampleSize);
	rm_acc_y = new RollingMedian<float>(sampleSize);
	rm_acc_z = new RollingMedian<float>(sampleSize);

	rm_us_f = new RollingMedian<float>(sampleSize);
	rm_us_r = new RollingMedian<float>(sampleSize);
	rm_us_l = new RollingMedian<float>(sampleSize);

	rm_ir = new RollingMedian<int>(sampleSize);

	update();
}

void WallyControl::update() {
	/* Update Timer */
	t1 = t2;
	t2 = micros();

	/* Update Rolling Medians */
	XYZ acc = wally->readAccelerometer();
	rm_acc_x->insertSample(acc.x);
	rm_acc_y->insertSample(acc.y);
	rm_acc_z->insertSample(acc.z);

	rm_us_f->insertSample(wally->readUltrasonic(0));
	rm_us_r->insertSample(wally->readUltrasonic(1));
	rm_us_l->insertSample(wally->readUltrasonic(2));

	rm_ir->insertSample(wally->readIR());
}

/*
Function: 		readAccelerometer
Description: 	Reads the median from the accelerometer
Returns:		XYZ struct of current accelerometer data
*/
XYZ WallyControl::readAccelerometer() {
	XYZ accData;

	accData.x = rm_acc_x->getMedian();
	accData.y = rm_acc_y->getMedian();
	accData.z = rm_acc_z->getMedian();

	return accData;
}

/*
Function:		readUltrasonic
Description:	Reads the median from an ultrasonic
Parameters:
	address:	Selects ultrasonic sensor -> 0=FRONT, 1=RIGHT, 2=LEFT
Returns:		Float of selected ultrasonic distance measurement	
*/
float WallyControl::readUltrasonic(int address) {
	if (address == 1) {
		return rm_us_r->getMedian();
	}
	
	if (address == 2)
		return rm_us_l->getMedian();

	return rm_us_f->getMedian();
}

/*
Function:		readIR
Description:	Reads the median IR sensor
Returns:		int status of IR sensor on=1, off=0 
*/
int WallyControl::readIR() {
	return rm_ir->getMedian();
}

/*
Function:		verticalControl
Description:	Run in your loop to give robot vertical control
*/
void WallyControl::verticalControl(float speed_r, float theta_r) {
	/* Collect Loop Variables */
	XYZ acc = wally->readAccelerometer();
	theta = wally->getTheta(acc);
	err2 = theta_r - theta;
	derr_dt = (err2-err1)/(t2-t1);

	/* Calculate Motor Inputs */
	/* Ensure input differential is within limits */
	float u = vp * err2 + vd * derr_dt;
	if (abs(u) > INPUT_LIMIT) {
		if (u > 0) {
			u = INPUT_LIMIT;
		} else {
			u = -INPUT_LIMIT;
		}
	}

	/* Ensure output is within limits */
	u = u / 2.0;
	if (speed_r + abs(u) > MOTOR_LIMIT) {
		speed_r = MOTOR_LIMIT - abs(u);
	} else if (speed_r - abs(u) < -MOTOR_LIMIT) {
		speed_r = -MOTOR_LIMIT + abs(u);
	}

	ur = speed_r + u;
	ul = speed_r - u;
	wally->setMotors(ul, ur);

	// Serial.print(ur);
	// Serial.print("   ");
	// Serial.println(ul);

	/* Update Stored Values */
	err1 = err2;
}

/*
Function:		horizontalControl
Description:	Run in your loop to give robot horizontal control
*/

void WallyControl::horizontalControl(float x_r) {
	/* Ensure output is within limits */
	float x = readUltrasonic(0);
	float u = hp * (x_r + x);
	
	if (u > MOTOR_LIMIT) {
		u = MOTOR_LIMIT;
	}

	/* Send output to motors */
	wally->setMotors(u, u);
}