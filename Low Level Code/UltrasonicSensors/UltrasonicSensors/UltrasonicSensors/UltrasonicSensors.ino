
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
int StateMachine;


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
	Ultra.initialize();
	// Attach int.0 interrupt at pin 2
	attachInterrupt(0,ultraInterrupt,CHANGE);
	StateMachine = 0;
}

/*********************************************************
*
*				Infinite Loop
*
**********************************************************/
void loop()
{
	//int temp = StateMachine;
	// Code for the break message:
	// {Ultra.EchoDistance[0]},{Ultra.EchoDistance[1]}, {Ultra.EchoDistance[2]},{Ultra.EchoDistance[3]},{Ultra.EchoDistance[4]},{Ultra.EchoDistance[5]}
	Ultra.spinOnce();
	
	switch (StateMachine)
	{
		case 0:
			Ultra.checkPoint(RIGHT,BACK,30,30,NEAR);
			if (Ultra.CheckPointFlag == 1){
				StateMachine++;}
			break;
		case 1:
			Ultra.forward();
			Ultra.forward();
			delay(5500);
			StateMachine++;
			Ultra.stop();
			break;
		case 2:
			Ultra.checkPoint(RIGHT,FRONT,30,30,FAR);
			if (Ultra.CheckPointFlag == 1){
				StateMachine++;}
			break;
		case 3:
			Ultra.turn90Cw();
			Ultra.turn90Cw();
			Ultra.forward();
			Ultra.forward();
			delay(5500);
			Ultra.turn90Cw();
			StateMachine++;
			break;
	}
	delay(1);
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


