// PCA9685-Arduino Servo Evaluator Example
// In this example, we utilize the ServoEvaluator class to assist with setting PWM
// frequencies when working with servos. We will be using Wire1, which is only available
// on boards with SDA1/SCL1 (e.g. Due/Mega/etc.) - change to Wire if Wire1 is unavailable.
#include <RichShieldKnob.h>
#include <Wire.h>
#include "RichShieldDHT.h"
#include "RichShieldTM1637.h"
#define KNOB_PIN A0
int knobValue;
#define LDR_PIN A2 
#define CLK 10 //CLK of the TM1637 IC connected to D10 of UNO R3/
#define DIO 11 //DIO of the TM1637 IC connected to D11 of UNO R3/
TM1637 disp(CLK, DIO);
#define LED_RED 4 //defining led pins 
#define LED_GREEN 5
#define LED_BLUE 6
DHT dht;
int r;
#include "PCA9685.h"
int aircon(float y);
float light(void);

PCA9685 pwmController(Wire);           // Library using Wire1 @400kHz, and default B000000 (A5-A0) i2c address
// Linearly interpolates between standard 2.5%/12.5% phase length (102/512) for -90°/+90°
PCA9685_ServoEval pwmServo1;

// Testing our second servo has found that -90° sits at 128, 0° at 324, and +90° at 526.
// Since 324 isn't precisely in the middle, a cubic spline will be used to smoothly
// interpolate PWM values, which will account for said discrepancy. Additionally, since
// 324 is closer to 128 than 526, there is slightly less resolution in the -90° to 0°
// range while slightly more in the 0° to +90° range.
PCA9685_ServoEval pwmServo2(128, 324, 526);

void setup() {
	{
		Serial.begin(9600);
		disp.init();
		dht.begin();
		light();
	}

	// Add the main program code into the continuous loop() function
	Serial.begin(115200);               // Begin Serial and Wire1 interfaces
	Wire.begin();

	pwmController.resetDevices();       // Resets all PCA9685 devices on i2c line

	pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer

	pwmController.setPWMFreqServo();    // 50Hz provides standard 20ms servo phase length

	pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(-90));
	Serial.println(pwmController.getChannelPWM(0)); // Should output 102 for -90°

	// Showing linearity for midpoint, 205 away from both -90° and 90°
	Serial.println(pwmServo1.pwmForAngle(0));   // Should output 307 for 0°

	pwmController.setChannelPWM(0, pwmServo1.pwmForAngle(90));
	Serial.println(pwmController.getChannelPWM(0)); // Should output 512 for +90°

	pwmController.setChannelPWM(1, pwmServo2.pwmForAngle(-90));
	Serial.println(pwmController.getChannelPWM(1)); // Should output 128 for -90°

	// Showing less resolution in the -90° to 0° range
	Serial.println(pwmServo2.pwmForAngle(-45)); // Should output 225 for -45°, 97 away from -90°

	pwmController.setChannelPWM(1, pwmServo2.pwmForAngle(0));
	Serial.println(pwmController.getChannelPWM(1)); // Should output 324 for 0°

	// Showing more resolution in the 0° to +90° range
	Serial.println(pwmServo2.pwmForAngle(45));  // Should output 424 for +45°, 102 away from +90°

	pwmController.setChannelPWM(1, pwmServo2.pwmForAngle(90));
	Serial.println(pwmController.getChannelPWM(1)); // Should output 526 for +90°
}

void loop() {
	{
		knobValue = analogRead(KNOB_PIN);
		while (knobValue > 570 || knobValue < 450) {

			if (knobValue > 570 && knobValue < 790) {
				int sum = aircon(light()) + 5;
				disp.display(sum);
				delay(1000);
			}
			else if (knobValue > 790) {
				float um = aircon(light()) + 10;
				disp.display(um);
				delay(1000);
			}
			else if (knobValue < 450 && knobValue>225) {
				int no = aircon(light()) - 5;
				disp.display(no);
				delay(1000);

			}
			else if (knobValue < 225) {
				int g = aircon(light()) - 5;
				disp.display(g);
				delay(1000);

			}
		}
	}

	float light() {


		while (true) {
			float l = dht.readTemperature();

			if (l <= 28 && l >= 27) {
				disp.display(l);
				delay(100);
				analogWrite(LED_GREEN, 255);
				aircon(l);
			}
			else if (l < 26) {
				disp.display(l);
				delay(100);
				analogWrite(LED_BLUE, 255);
				aircon(l);

			}
			else if (l >= 29) {
				disp.display(l);
				delay(100);
				analogWrite(LED_RED, 255);
				aircon(l);
			}

			return l;
		}

	} int aircon(float y) {
		if (y <= 28 && y >= 27) {
			disp.clearDisplay();
			disp.display(18);
		}
		else if (y < 26) {
			disp.clearDisplay();
			disp.display(22);

		}
		else if (y >= 29) {
			disp.clearDisplay();
			disp.display(15);
		}

		return y;
		for (i = 0; i <= 90; i += 5) { //Slow Turn anti-clockwise (from 0 to 90 degree
			pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
			delay(250); //longer delay for Slow turn movement
		}
		for (i = 90; i >= 0; i -= 5) { //Fast Turn clockwise (from 90 to 0 degree)
			pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
			delay(100); //shorter delay for faster turn movement
		}
		for (i = 0; i >= -90; i -= 5) { //Slow Turn clockwise (from 0 to -90 degree)
			pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
			delay(250); //longer delay for Slow turn movement
		}
		for (i = -90; i <= 0; i += 5) { //Fast Turn anti-clockwise (from -90 to 0 degree)
			pwmController.setChannelPWM(1, pwmServo1.pwmForAngle(i));
			delay(100); //shorter delay for faster turn movement
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
}
