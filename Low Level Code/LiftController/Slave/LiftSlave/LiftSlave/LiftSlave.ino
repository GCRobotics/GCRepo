#include "Adafruit_MotorShield.h"
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
#include "Lift.h"

#define LIFT_SPEED 600
#define ARM_SPEED  200
#define STEPS_TO_FRIDGE		4000
#define STEPS_TO_SWITCH		2000
#define STEPS_TO_CLOSE		1000
#define STEPS_FROM_SWITCH	1000
#define STEPS_FROM_FRIDGE	1000
#define STEPS_TO_TABLE		4000
#define STEPS_TO_TABLE_TOP	1000
#define STEPS_TO_OPEN		1000

#define LIFT_LED 7
#define ARM_LED  6


//Adafruit_MotorShield LiftArm = Adafruit_MotorShield(0x61);
//Adafruit_MotorShield Arm = Adafruit_MotorShield(0x60);
//
//Adafruit_StepperMotor *LiftMotor = LiftArm.getStepper(200, 1);
//Adafruit_StepperMotor *ArmMotor = Arm.getStepper(100, 1);

int StateMachine = 0;
int Level = 0, Data = 0;
Lift Slave; 


void setup()
{
	// Setup default for steppers
	//LiftArm.begin();
	//Arm.begin();
	//// Set the rpm of the stepper
	//LiftMotor -> setSpeed(LIFT_SPEED);
	//ArmMotor ->setSpeed(ARM_SPEED);
	////LiftStepper -> setSpeed(100);
	pinMode(13,OUTPUT);
	digitalWrite(13,LOW);
	pinMode(LIFT_LED,OUTPUT);
	digitalWrite(LIFT_LED,LOW);
	pinMode(ARM_LED,OUTPUT);
	digitalWrite(ARM_LED,LOW);
	//Slave.SlaveInitialize();
	//Level = Slave.SlaveGetLevel();
}

void loop()
{
	delay(1);
	switch (StateMachine)
	{
		case 0:
			if (Slave.SlaveStateChangeCheck() == 1)
			{
				Slave.SlaveSetBusy();
				digitalWrite(LIFT_LED,HIGH);
				
				// Lift to the height of the fridge plus a little bit
				//LiftMotor -> step(STEPS_TO_FRIDGE, FORWARD, DOUBLE);
				
				delay(3000);
				digitalWrite(LIFT_LED,LOW);
				Slave.SlaveSetReady();
				StateMachine++;
			}
			break;
		case 1:
			if (Slave.SlaveStateChangeCheck() == 1)
			{
				Slave.SlaveSetBusy();
			
			
				//Raise that one flag!!!!!!!!!
			
			
				digitalWrite(LIFT_LED,HIGH);
				delay(2000);
				digitalWrite(LIFT_LED,LOW);
				digitalWrite(ARM_LED,HIGH);
				delay(2000);
				digitalWrite(ARM_LED,LOW);
				digitalWrite(LIFT_LED,HIGH);
				delay(2000);
				digitalWrite(LIFT_LED,LOW);
				
				//// Open the arm
				//ArmMotor -> step(STEPS_TO_OPEN, BACKWARD, DOUBLE);
				//// Lower till limit switch
				//LiftMotor -> step(STEPS_TO_SWITCH, BACKWARD, DOUBLE);
				//// Raise up a little bit
				//LiftMotor -> step(STEPS_FROM_SWITCH, FORWARD, DOUBLE);
				//// Close the arm
				//ArmMotor -> step(STEPS_TO_CLOSE, FORWARD, DOUBLE);
				//// Lift up by a little bit
				//LiftMotor -> step(STEPS_FROM_FRIDGE, FORWARD, DOUBLE);
				
				Slave.SlaveSetReady();
				StateMachine++;
			}
			break;
		// this is just to make sure that the robot wait for the lift to be done
		case 2:
			if (Slave.SlaveStateChangeCheck() == 1)
			{
				Slave.SlaveSetBusy();
				delay(100);
				Slave.SlaveSetReady();
				StateMachine++;
			}
			break;
		case 3:
			if (Slave.SlaveStateChangeCheck() == 1)
			{
				Slave.SlaveSetBusy();
				digitalWrite(LIFT_LED,HIGH);
				
				//// lOWER Lift to the height of the table plus a little bit
				//LiftMotor -> step(STEPS_TO_TABLE, BACKWARD, DOUBLE);
				
				delay(3000);
				digitalWrite(LIFT_LED,LOW);
				Slave.SlaveSetReady();
				StateMachine++;
			}
			break;
		case 4:
			if (Slave.SlaveStateChangeCheck() == 1)
			{
				Slave.SlaveSetBusy();
				digitalWrite(LIFT_LED,HIGH);
				delay(2000);
				digitalWrite(LIFT_LED,LOW);
				digitalWrite(ARM_LED,HIGH);
				delay(2000);
				digitalWrite(ARM_LED,LOW);
				
				//// Lower the lift to the height 
				//LiftMotor -> step(STEPS_TO_TABLE_TOP, BACKWARD, DOUBLE);
				//// Open the arm 
				//ArmMotor -> step(STEPS_TO_OPEN, BACKWARD, DOUBLE);

				Slave.SlaveSetReady();
				StateMachine++;
			}
			break;
	}
	
}

