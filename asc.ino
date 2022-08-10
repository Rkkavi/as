
#include "PCA9685.h"

PCA9685 pwmController(Wire);
PCA9685_ServoEval pwmServo1;

// Testing our second servo has found that -90° sits at 128, 0° at 324, and +90° at 526.
// Since 324 isn't precisely in the middle, a cubic spline will be used to smoothly
// interpolate PWM values, which will account for said discrepancy. Additionally, since
// 324 is closer to 128 than 526, there is slightly less resolution in the -90° to 0°
// range while slightly more in the 0° to +90° range.
//PCA9685_ServoEval pwmServo2(128, 324, 526);

void setup() {
	Wire.begin();

	pwmController.resetDevices();       // Resets all PCA9685 devices on i2c line

	pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer

	pwmController.setPWMFreqServo();    // 50Hz provides standard 20ms servo phase length

	// bring to starting position
	pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(-10));//connected to upper servo
	pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(-90)); //connected to lower servo

	delay(1000);

}
int i;
void loop() {

	for (i = -90; i < 90; i += 0.5)
	{
		pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));//lower servo
		delay(1);
	}
	while (i > -90)
	{
		pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));//lower servo
		i += 0.5;
		delay(1);
	}
	for (i = -10; i < 90; i += 0.5)
	{
		pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(i));// upper servo (degree of turn is limited (from -10 degree)
		delay(200);
	}
	while (i > -10)
	{
		pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(i));//upper servo
		i -= 10;
		delay(100);
	}

}