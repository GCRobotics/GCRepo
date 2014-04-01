/*
 * Demonstration.ino
 *
 * Created: 3/30/2014 10:16:11 PM
 * Author: Quang
 */ 
/********************************************************
*				Includes
*********************************************************/
#include <Wire.h>
#include "Motors.h"

/********************************************************
*				Define Definition 
*********************************************************/
#define STOP_PERIOD 500
/********************************************************
*				Global Declarations
*********************************************************/
int StateMachine = 0;

int YTarget = 0;
int Xtarget = 0;
int WTarget = 0;
int TargetFlag = 0;

/********************************************************
*				Object Declarations
*********************************************************/
Motors RobotMove;

/********************************************************
*				Setup Function
*********************************************************/
void setup()
{
	  Wire.begin();
	  
	  //Setting up for robot to move forward by 115 cm 
	  YTarget = 4989;	  
}

/********************************************************
*				Infinite loop
*********************************************************/
void loop()
{
	switch (StateMachine)
	{
		// Assuming we are starting at (62,35)
		// move forward by 155 cm or 1550 mm
		case 0:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			else {
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(STOP_PERIOD);
			}
			break;
		// Grabbing the plate
		case 1:
			delay(2000);
			StateMachine++;
			YTarget = 2414;
			break;
		// move backward for 75 cm
		case 2:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveBackward(&YTarget);
			}
			else {
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(STOP_PERIOD);
				WTarget = 1378;
			}
			break;
		// Turn 90* CCW
		case 3:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveCCW(&WTarget);
			}
			else {
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(STOP_PERIOD);
				YTarget = 3798;
			}
			break;
		// move forward by 118 cm 
		case 4:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			else {
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(STOP_PERIOD);
				WTarget = 1378;
			}
			break;
		// Turn 90* CCW
		case 5:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveCCW(&WTarget);
			}
			else {
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(STOP_PERIOD);
				YTarget = 805;
			}
			break;
		// move forward by 25 cm
		case 6:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			else {
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(STOP_PERIOD);
				WTarget = 1124;
			}
			break;
		// Put down the plate
		case 7:
			delay(2000);
			StateMachine++;
			break;
		default:
			RobotMove.stop();	
	}
}
