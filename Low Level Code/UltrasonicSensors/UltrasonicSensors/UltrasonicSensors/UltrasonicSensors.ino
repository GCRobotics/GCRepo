
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
	//ForwardTarget = 305;
	XTarget = 1124;
	YTarget = 966;
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
		StateMachine++;}
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


