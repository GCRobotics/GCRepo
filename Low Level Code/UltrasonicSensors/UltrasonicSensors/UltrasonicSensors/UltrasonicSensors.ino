
/*
 * UltrasonicInterrupt.ino
 *
 * Created: 3/14/2014 7:22:23 PM
 * Author: Quang
 */ 
#include "Ultrasonic.h"
#include <Wire.h>
#include "Motors.h"
	
	
/********************************************************
*          Constants for Ultrasonic Sensors
*********************************************************/








// Target Variables
int SensorTargetRight = 30, SensorTargetBack = 40;
int StateMachine = 0;

int YTarget = 0;
int XTarget = 0;
int WTarget = 0;
int TargetFlag = 0;


// Create instance of the class Ultrasonic
Ultrasonic Ultra;
// Create instance of the class Motors
Motors RobotMove;


/*********************************************************
*
*				Setup Loop
*
**********************************************************/
void setup()
{
	Wire.begin();
	// Attach int.0 interrupt at pin 2
	attachInterrupt(0,ultraInterrupt,CHANGE);
	Ultra.initialize();
	pinMode(13,OUTPUT);
	digitalWrite(13, LOW);
	//ForwardTarget = 305;
	//XTarget = 1124;
	//YTarget = 1914;
	//WTarget = 2362;
	//Ultra.Select = 0;

	delay(100);

}

/*********************************************************
*
*				Infinite Loop
*
**********************************************************/
void loop()
{	
 //Testing RobotMove routine
 //move forward 1 foot and then turn 180*
 
 /*
	if (StateMachine == 0)
	{
		if (TargetFlag == 0){
			TargetFlag = RobotMove.moveCCW(&WTarget);
		}
		if (TargetFlag == 1){
			TargetFlag = 0;
			StateMachine++;
			delay(20);
		}
	}
	else
	{
		RobotMove.stop();
		delay(300);
	}
 */
 
 
// Working Statemachine
	//int temp = StateMachine;
	// Code for the break message:
	// {Ultra.EchoDistance[0]},{Ultra.EchoDistance[1]}, {Ultra.EchoDistance[2]},{Ultra.EchoDistance[3]},{Ultra.EchoDistance[4]},{Ultra.EchoDistance[5]}
	//Ultra.spinOnce(BACK);
	
	// First state = get off the starting block by moving to (52, 65)
		// X = 52 pretty much line the robot up with the plate
	__asm__("nop\n\t");
	switch (StateMachine)
	{
		//Start at [(52,65)]
		case 0:
			Ultra.checkPoint(BACK,52,65);
			
			if (Ultra.CheckPointFlag == 1){
				StateMachine++;
				Ultra.StateMachine = 0;
				YTarget = 3611;
			}
			break;
		// Go to [52,180] by moving 115 cm forward
		case 1:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
			}
			break;
		// Check point at [52,180] by (52,65)
		case 2:
			//Ultra.checkPoint(FRONT, 52,65);
			// Compensating for the irregular back of the fridge
			Ultra.checkPoint(FRONT,52,41);
			if (Ultra.CheckPointFlag == 1){
				Ultra.StateMachine = 0;
				StateMachine++;
				YTarget = 314;
			}
			break;
		// Move to [52,190] by moving forward by 10 cm 
		case 3:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				// Blink LED to indicate that we are grabbing the plate
				delay(1000);
				digitalWrite(13, HIGH);
				delay(1000);
				digitalWrite(13, HIGH);
				
				YTarget = 2826;
			}
			break;
		// Move to [52,100] by moving backward 90 cm
		case 4:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveBackward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				WTarget = 1181;
			}
			break;
		// Rotate 90* CCW
		case 5:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveCCW(&WTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				YTarget = 4019;
			}
			break;
		//Move to [180,100] by moving forward by 128 cm 
		case 6:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				WTarget = 1181;
			}
		// Rotate 90* CCW
		case 7:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveCCW(&WTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				YTarget = 785;
			}
			break;
		//Move to [180,75] by moving forward by 25 cm
		case 8:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
			}
			break;
		// Check point at [180,75] by (65,70)
		case 9:
			Ultra.checkPoint(FRONT,65,70);
			if (Ultra.CheckPointFlag == 1){
				Ultra.StateMachine = 0;
				StateMachine++;
				YTarget = 314;
			}
			break;
		//Move to [180,65] by moving forward by 10 cm
		case 10:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
			}
		default:
			RobotMove.stop();
			delay(40);
			break;
	}
	
}

/*********************************************************
*
*				Ultrasonic Interrupt Routine
*
**********************************************************/
void ultraInterrupt()
{
	Ultra.interrupt();
}


