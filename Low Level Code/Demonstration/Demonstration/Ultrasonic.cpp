/*
* Ultrasonic.cpp
*
* Created: 3/15/2014 4:32:08 PM
* Author: Quang
*/

#include "Ultrasonic.h"
#include <Arduino.h>
#include "Motors.h"

Motors Robot;

// default constructor
Ultrasonic::Ultrasonic()
{
	// Initializing everything with zero
	
	PreviousTime =0;
	InterruptEchoTime = 0;
	Select = 0;
	for (int i = 0; i < 6; i++)
	{
		EchoDistance[i] = 0;
	}
	FullSet = 0;
	StateMachine = 0;
	CheckPointFlag = 0;
	TargetFlag = 0;
} //Ultrasonic

// default destructor
Ultrasonic::~Ultrasonic()
{
} //~Ultrasonic


void Ultrasonic::initialize()
{
	// Set echo pin as input
	pinMode(ECHO_PIN, INPUT);
	// set trigger pin as output
	pinMode(TRIGGER_PIN, OUTPUT);
	
	
	// Set the address pins as output
	pinMode(ADDR0_PIN, OUTPUT);
	pinMode(ADDR1_PIN, OUTPUT);
	pinMode(ADDR2_PIN, OUTPUT);
	
	// Set these pin to LOW to begin with
	digitalWrite(ADDR0_PIN, LOW);
	digitalWrite(ADDR1_PIN, LOW);
	digitalWrite(ADDR2_PIN, LOW);
	digitalWrite(TRIGGER_PIN, LOW);
}

void Ultrasonic::spinOnce()
{
	if ((Select == 0) || (Select == 2) || (Select == 4) ||
	(Select == 6) || (Select == 8) || (Select == 10) || (Select == 12))
	{
		// Record the ultrasonic sensor.
		// Skip the first one since there is nothing to read
		if (Select > 0)
		{
			// The index of the array is equal to (Select/2) +1
			// Ex.
			// We want to store of the first ultrasonic sensor, which is at Select = 2
			// index = (2/2) - 1 = 0  <-- that's is the index that we want
			// 
			// Distance (in cm) = Time / 58   <--  According to the datasheet
			
			// Don't store the sensor data if it exceed 8 feet or 244
			int Temp = InterruptEchoTime / 58;
			if ((Temp >= 0) && (Temp <= MAX_DISTANCE))
			{
				EchoDistance[((Select/2) - 1)] = Temp;
			}
		}
		
		// Reset to zero at the value of 12
		if (Select >= 12)
		{
			Select = 0;
			// Set FullSet high to indicate that we have now filled the complete array
			// this variable is so that the robot doesn't start moving with the array not completed
			FullSet++; 
		}
		
		// Set the address pins to the correct address
		addressSelect(Select);
		// Set the trigger pin to HIGH for 10 us
		trigger();
		// Increment to odd
		Select++;
	}
}

void Ultrasonic::interrupt()
{
	// If it just went from lOW to HIGH, then this is the beginning of the echo cycle
	if (digitalRead(ECHO_PIN) == 1) 
	{
		PreviousTime = micros();
	}
	
	// If it just went from HIGH to LOW, then this is the end of the echo cycle
	else if (digitalRead(ECHO_PIN) == 0)
	{
		InterruptEchoTime = micros() - PreviousTime;
		//Increment the select variable to even
		Select++;
	}
}

void Ultrasonic::trigger()
{
	// According to the data sheet, we need to set trigger to high for 10 us
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER_PIN,LOW);
}

void Ultrasonic::addressSelect(int Address)
{
	switch (Address)
	{
		case 0:
		pinSelect(0);
		break;
		case 2:
		pinSelect(1);
		break;
		case 4:
		pinSelect(2);
		break;
		case 6:
		pinSelect(3);
		break;
		case 8:
		pinSelect(4);
		break;
		case 10:
		pinSelect(5);
		break;
		
	}
}

void Ultrasonic::pinSelect (int Address)
{
	// Set the least sig. digit
	digitalWrite(ADDR0_PIN, (Address & 0x01));			// & is to mask the bits
	// Set the middle digit
	digitalWrite(ADDR1_PIN, (Address & 0x02));
	// Set the most sig. digit
	digitalWrite(ADDR2_PIN, (Address & 0x04));
}

//
//void Ultrasonic::checkPoint(char Side, char Face, int XTarget, int YTarget, int Near)
//{
	//CheckPointFlag = 0;
	//// If we have finally collect the first set of the ultrasonic data
	//if (FullSet == 1)
	//{
		//int XCurrent1 = EchoDistance[Side ? 0:4];
		//int XCurrent2 = EchoDistance[Side ? 1:3];
		//int YCurrent  = EchoDistance[Face ? 5:2];
		//
		//// Making sure that we have the correct orientation
		////if (StateMachine == 0)
		////{
			////// Side that we are using should always be smaller than the side that we don't want
			////// If it is bigger then just rotate clockwise until you are at the correct orientation
			////if (EchoDistance[Side ? 0:4] > EchoDistance[Side ? 4:0])
				////Robot.cw(FastSpeed);
			////else if (EchoDistance[Face ? 5:2] > EchoDistance[Side ? 2:5])
				////Robot.cw(FastSpeed);
			////else
				////StateMachine++;
		////}
		//
		//// Making sure both sides are parallel
		//if (StateMachine == 0)
		//{
			//// If the difference of the 2 right sensors are greater than the tolerance
			//if ( ((XCurrent1 - XCurrent2) > SENSOR_TOLERANCE) || ((XCurrent1 - XCurrent2) < -SENSOR_TOLERANCE) )
			//{
				//// Determining if robot needs to rotate CW or CCW
				//if (XCurrent1 > XCurrent2)
				//{
					//Robot.cw(SlowSpeed);
				//}
				//else
				//{
					//Robot.ccw(SlowSpeed);
				//}
			//}
			//// Stop the robot and move to the next state if the robot parallel
			//else {
				////Robot.stop();
				//StateMachine++;
			//}
		//}
		//
		//// Making sure the robot reaches the x-axis target
		//if (StateMachine == 1)
		//{
			//// If the Front right sensor is greater than the X-Target
			//// We are only using one of the sensor to check for distance
			//if ((XCurrent1 < (XTarget - SENSOR_TOLERANCE)) || (XCurrent1 > (XTarget + SENSOR_TOLERANCE)))
			//{
				//
			///*************************
			//*
			//* This is not needed on the real robot
			//* We need it right now because strafing is not really "strafing"
			//*
			//************************/
			//if (YCurrent >= 20)
			//{
				//// Determining if the robot needs to strafe left or right
				//if (XCurrent1 < (XTarget - SENSOR_TOLERANCE))
				//{
					//Robot.left(SlowSpeed);
				//}
				//else
				//{
					//Robot.right(SlowSpeed);
				//}
				//
			//}
			//else {
				//StateMachine = 2;}
				////StateMachine = 0;
			//}
			//
			//// The robot is at the x-axis target
			//else
			//{
				//// Determine if the robot is at the x-axis target && is parallel to the wall
				//if ((XCurrent1 >= (XTarget - SENSOR_TOLERANCE)) && (XCurrent1 <= (XTarget + SENSOR_TOLERANCE))
				//&&
				//(XCurrent2 >= (XTarget - SENSOR_TOLERANCE)) && (XCurrent2 <= (XTarget + SENSOR_TOLERANCE)))
				//{
					////stop the robot and go to the next state
					////Robot.stop();
					//StateMachine++;
				//}
				//// else the robot needs to go back to state 0 and try to get it self parallel to the wall
				//else
				//{
					//StateMachine--;
				//}
			//}
		//}
		//
		//// Making sure the robot reaches the y-axis
		//if (StateMachine == 2)
		//{
			//// Making sure the back sensor is outside the target value
			//if ((YCurrent < (YTarget - SENSOR_TOLERANCE)) || (YCurrent > (YTarget + SENSOR_TOLERANCE)))
			//{
				//// Deciding if the robot needs to move forward or backward
				//if (YCurrent < (YTarget - SENSOR_TOLERANCE))
				//{
					//if (Near == 1)
						//Robot.forward(SlowSpeed);
					//else 
						//Robot.backward(SlowSpeed);
				//}
				//else
				//{
					//if (Near == 1)
						//Robot.backward(SlowSpeed);
					//else 
						//Robot.forward(SlowSpeed);
				//}
				////StateMachine-- ;
			//}
			//// The robot is at the y-axis target
			//else
			//{
				//// Determining if the robot is (parallel to the wall) && (at the x-axis target) && (at the y-axis target)
				//if (
				//(XCurrent1 >= (XTarget - SENSOR_TOLERANCE)) &&
				//(XCurrent1 <= (XTarget + SENSOR_TOLERANCE)) &&
				//(XCurrent2 >= (XTarget - SENSOR_TOLERANCE)) &&
				//(XCurrent2 <= (XTarget + SENSOR_TOLERANCE)) &&
				//(YCurrent >= (XTarget - SENSOR_TOLERANCE)) &&
				//(YCurrent <= (XTarget + SENSOR_TOLERANCE))   )
				//{
					////stop the robot and go to the next state
					//Robot.stop();
					//
					///*************************
					//*
					//* Need to change back to next state
					//*
					//************************/
					//StateMachine = 0;
					//CheckPointFlag = 1;
				//}
				//// else the robot needs to go back to state 0 and try to get it self parallel to the wall
				//else
				//{
					//StateMachine = 0;
				//}
			//}
		//}
	//}
//}
//

void Ultrasonic::checkPoint(char Side, char Face, int XTarget, int YTarget, int Near)
{
	CheckPointFlag = 0;
	// If we have finally collect the first set of the ultrasonic data
	if (FullSet >= 4)
	{
		int XCurrent1 = X_OFFSET + EchoDistance[Side ? 0:4];
		int XCurrent2 = X_OFFSET + EchoDistance[Side ? 1:3];
		int YCurrent  = Y_OFFSET + EchoDistance[Face ? 5:2];
		
		// Making sure that we have the correct orientation
		//if (StateMachine == 0)
		//{
			//// Side that we are using should always be smaller than the side that we don't want
			//// If it is bigger then just rotate clockwise until you are at the correct orientation
			//if (EchoDistance[Side ? 0:4] > EchoDistance[Side ? 4:0])
				//Robot.cw(FastSpeed);
			//else if (EchoDistance[Face ? 5:2] > EchoDistance[Side ? 2:5])
				//Robot.cw(FastSpeed);
			//else
				//StateMachine++;
		//}
		
		// Making sure both sides are parallel
		if (StateMachine == 0)
		{
				// How far we are away is equal to difference b/w the XTarget and the average of the 2 side sensors
				WError = XCurrent1 - XCurrent2;
				// Decide decide whether we need to move away or towards the target
				if (WError > 0)
					DirectionFlag = 1;
				else
					DirectionFlag = 0;
				// Convert cm to counts
				XError = abs(WError) * DEGREES_TO_MOVE;
				StateMachine++;
		}
		if (StateMachine == 1)
		{
			// if we need to move toward the target
			if (TargetFlag == 0)
			{
				// Move Right till the value of counts in XError is reached
				if (DirectionFlag == 1)
					TargetFlag = Robot.moveCW(&WError);
				else
					TargetFlag = Robot.moveCCW(&WError);
			}
			else 
			{
				TargetFlag = 0;
				delay(5);
			}
			
			// If the difference of the 2 right sensors are greater than the tolerance
			if ( ((XCurrent1 - XCurrent2) < PARALLEL_TOLERANCE) && ((XCurrent1 - XCurrent2) > -PARALLEL_TOLERANCE) )
			{
				//Robot.stop();
				StateMachine++;
				
				// How far we are away is equal to difference b/w the XTarget and the average of the 2 side sensors
				XError = XTarget - ((XCurrent1 + XCurrent2) >> 1);
				// Decide decide whether we need to move away or towards the target
				if (XError > 0)
					DirectionFlag = 0;
				else
					DirectionFlag = 1;
				// Convert cm to counts
				XError = abs(XError) * X_COUNTS_PER_CM;
			}
		}
		
		// Making sure the robot reaches the x-axis target
		if (StateMachine == 2)
		{
			// If the Front right sensor is greater than the X-Target
			// We are only using one of the sensor to check for distance

			// if we need to move toward the target
			if (TargetFlag == 0){
				// Move Right till the value of counts in XError is reached
				if (DirectionFlag == 1)
					TargetFlag = Robot.moveRight(&XError);
				else 
					TargetFlag = Robot.moveLeft(&XError);
			}
			else {
				TargetFlag = 0;
				delay(5);
			}
			// Determine if the robot is at the x-axis target && is parallel to the wall
			if ((XCurrent1 >= (XTarget - SENSOR_TOLERANCE)) && (XCurrent1 <= (XTarget + SENSOR_TOLERANCE))
			&&
			(XCurrent2 >= (XTarget - SENSOR_TOLERANCE)) && (XCurrent2 <= (XTarget + SENSOR_TOLERANCE)))
			{
				//stop the robot and go to the next state
				//Robot.stop();
				StateMachine++;
				// How far we are away is equal to difference b/w the XTarget and the average of the 2 side sensors
				YError = YTarget - YCurrent;
				// Decide decide whether we need to move away or towards the target
				if (YError > 0)
					DirectionFlag = 0;
				else
					DirectionFlag = 1;
				// Convert cm to counts
				YError = abs(YError) * Y_COUNTS_PER_CM;
			}
			// else the robot needs to go back to state 0 and try to get it self parallel to the wall
			else
			{
				StateMachine = 0;
			}
		}
		
		// Making sure the robot reaches the y-axis
		if (StateMachine == 3)
		{
			// Making sure the back sensor is outside the target value

			// if we need to move toward the target
			if (TargetFlag == 0){
				// Move Right till the value of counts in XError is reached
				if (DirectionFlag == 1)
				TargetFlag = Robot.moveBackward(&YError);
				else
				TargetFlag = Robot.moveForward(&YError);
			}
			else {
				TargetFlag = 0;
				delay(5);
			}

			// Determining if the robot is (parallel to the wall) && (at the x-axis target) && (at the y-axis target)
			if (
			(XCurrent1 >= (XTarget - SENSOR_TOLERANCE)) &&
			(XCurrent1 <= (XTarget + SENSOR_TOLERANCE)) &&
			(XCurrent2 >= (XTarget - SENSOR_TOLERANCE)) &&
			(XCurrent2 <= (XTarget + SENSOR_TOLERANCE)) &&
			(YCurrent >= (XTarget - SENSOR_TOLERANCE)) &&
			(YCurrent <= (XTarget + SENSOR_TOLERANCE))   )
			{
				//stop the robot and go to the next state
				Robot.stop();
					
				/*************************
				*
				* Need to change back to next state
				*
				************************/
				StateMachine = 0;
				CheckPointFlag = 1;
			}
			// else the robot needs to go back to state 0 and try to get it self parallel to the wall
			else
			{
				StateMachine = 0;
			}
		}
	}
}
