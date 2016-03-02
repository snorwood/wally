#include "wally.h"

Wally::Wally() {
	mma = new Adafruit_MMA8451();
	us_f = new NewPing(US_F_T, US_F_E, US_MAX_DISTANCE);
	us_r = new NewPing(US_R_T, US_R_E, US_MAX_DISTANCE);
}

float Wally::convertAccelerometer(int count) {
  return count * ADC_CONVERT;
}

void Wally::calibrateAccelerometer(){
	accelerometerOffset.x = 0;
	accelerometerOffset.y = 0;
	accelerometerOffset.z = 0;
}

XYZ Wally::readAccelerometer() {
	XYZ accData;
	
	mma->read();
	accData.x = convertAccelerometer(mma->x);
	accData.y = convertAccelerometer(mma->y);
	accData.z = convertAccelerometer(mma->z); 

	return accData;
}

float Wally::readUltrasonic(int address) {
	if (address == 0)
		return us_f->ping() / float(US_CONVERT);

	return us_r->ping() / float(US_CONVERT);
}