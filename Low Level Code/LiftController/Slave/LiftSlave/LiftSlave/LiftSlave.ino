#include "Adafruit_MotorShield.h"
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
#include "Lift.h"


Adafruit_MotorShield Lift = Adafruit_MotorShield(0x61);
Adafruit_MotorShield Arm = Adafruit_MotorShield(0x60);

Adafruit_StepperMotor *LiftMotor = Lift.getStepper(200, 1);
Adafruit_StepperMotor *ArmMotor = Arm.getStepper(100, 1);

int Level = 0, Data = 0;
Lift Slave; 

void setup()
{
	// Setup default for steppers
	Lift.begin();
	Arm.begin();
	//// Set the rpm of the stepper
	LiftMotor -> setSpeed(200);
	ArmMotor ->setSpeed(200);
	//LiftStepper -> setSpeed(100);
	pinMode(13,OUTPUT);
	digitalWrite(13,LOW);
	Slave.SlaveInitialize();
	Level = Slave.SlaveGetLevel();
}

void loop()
{
	if (Slave.SlaveStateChangeCheck() == 1)
	{
		Slave.SlaveSetBusy();
		//LiftStepper -> step(300, FORWARD, SINGLE);
		//LiftStepper -> step(400, FORWARD, SINGLE);
		digitalWrite(13,HIGH);
		delay(5000);
		digitalWrite(13,LOW);
		Slave.SlaveSetReady();
	}
}

