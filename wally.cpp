#include "wally.h"
#include <Adafruit_MMA8451.h>
#include <NewPing.h>

float adcConvert(int count) {
  return count * ADC_CONVERT;
}

Adafruit_MMA8451 mma = Adafruit_MMA8451();
XYZ accelerometerOffset;
NewPing us_f = NewPing(US_F_T, US_F_E, US_MAX_DISTANCE);
NewPing us_r = NewPing(US_R_T, US_R_E, US_MAX_DISTANCE);

void calibrateAccelerometer(){
	accelerometerOffset.x = 0;
	accelerometerOffset.y = 0;
	accelerometerOffset.z = 0;
}

XYZ readAccelerometer() {
	XYZ accData;
	
	mma.read();
	accData.x = adcConvert(mma.x);
	accData.y = adcConvert(mma.y);
	accData.z = adcConvert(mma.z); 

	return accData;
}

float readUltrasonic(int address) {
	if address == 0
		return us_f.ping() / float(US_CONVERT);

	return us_r.ping() / float(US_CONVERT);
}