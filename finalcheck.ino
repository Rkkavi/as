#include <Wire.h>
#include "PCA9685.h"
/*Create an object, pwmController of class PCA9685 and initialise it with Wire, as
defined in TWI/I2C library for Arduino & Wiring */
PCA9685 pwmController(Wire);
/*Create an object, pwmServo1 of class PCA9685_ServoEval to assist with
calculating Servo PWM values from angle/speed done internally by the library
files*/
PCA9685_ServoEval pwmServo1;
void setup() {

	pwmController.resetDevices(); // Resets all PCA9685 devices on i2c line
   /* Initializes module using default totem-pole driver mode, and default disabled
   phase balancer */
	pwmController.init();
	// Set standard servo frequency of 50Hz (or 20ms servo phase length)
	pwmController.setPWMFreqServo();

		/* Bring to starting position as defined by the programmer based on how the servos
		were mounted and the application */
		/* Channel 0 connected to upper Servo. Angle value Limited to -10 degree (minimum
		value in this application) due to its movement blocked by the lower bracket/servo
		(mechanical design) */
		pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(-10));

	/*Channel 1 connected to lower servo. Angle value is set at 0 degree, at the
	middle of the normal range from -90 to +90 */
	pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(0));
	delay(1000);
}
int i; //for loop control
void loop() {

	//Lower Servo (Channel 1) Control
	for (i = 0; i <= 90; i += 5) { //Slow Turn anti-clockwise (from 0 to 90 degree
		pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
		delay(250); //longer delay for Slow turn movement
	}
	for (i = 90; i >= 0; i -= 5) { //Fast Turn clockwise (from 90 to 0 degree)
		pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
		delay(250); //shorter delay for faster turn movement
	}
	for (i = 0; i >= -90; i -= 5) { //Slow Turn clockwise (from 0 to -90 degree)
		pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
		delay(250); //longer delay for Slow turn movement
	}
	for (i = -90; i <= 0; i += 5) { //Fast Turn anti-clockwise (from -90 to 0 degree)
		pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
		delay(250); //shorter delay for faster turn movement
	}
	//Upper servo (Channel 0) control
	for (i = -10; i <= 90; i += 5) { //Slow Turn going downward (from -10 to 90 degree
	// upper servo (degree of turn is limited (from -10 degree)
		pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(i));
		delay(250); //longer delay for Slow turn movement
	}
	for (i = 90; i >= -10; i -= 5) { //Fast Turn raising up (from 90 to -10 degree)
		pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(i));//upper servo
		delay(100); //shorter delay for faster turn movement
	}
}