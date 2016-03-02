#include "wally.h"
#include <Adafruit_MMA8451.h>
#include <NewPing.h>

float adcConvert(int count) {
  return count * ADC_CONVERT;
}

Adafruit_MMA8451 mma = Adafruit_MMA8451();
XYZ accelerometerOffset;

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
	return 1;
}