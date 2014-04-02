
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
	XTarget = 1124;
	YTarget = 1957;
	WTarget = 2248;
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
			TargetFlag = RobotMove.moveLeft(&XTarget);
		}
		if (TargetFlag == 1){
			TargetFlag = 0;
			StateMachine++;
			delay(20);
		}
	}
	else if (StateMachine == 1)
	{
		if (TargetFlag == 0){
			TargetFlag = RobotMove.moveCW(&WTarget);
		}
		if (TargetFlag == 1) {
			TargetFlag = 0;
			StateMachine++;
			delay(5);
		}
	}
	else
		RobotMove.stop();
		*/
 
 
 
// Working Statemachine
	//int temp = StateMachine;
	// Code for the break message:
	// {Ultra.EchoDistance[0]},{Ultra.EchoDistance[1]}, {Ultra.EchoDistance[2]},{Ultra.EchoDistance[3]},{Ultra.EchoDistance[4]},{Ultra.EchoDistance[5]}
	//Ultra.spinOnce(BACK);
	switch (StateMachine)
	{
		case 0:
			Ultra.checkPoint(BACK,30,30);
			if (Ultra.CheckPointFlag == 1){
				StateMachine++;
				Ultra.StateMachine = 0;
				delay(20);
			}
			break;
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
		case 2:
			Ultra.checkPoint(FRONT,40,40);
			if (Ultra.CheckPointFlag == 1){
				StateMachine++;
				delay(20);
			}
			break;
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


