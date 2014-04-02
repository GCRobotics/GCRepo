/*
* Ultrasonic.h
*
* Created: 3/15/2014 4:32:08 PM
* Author: Quang
*/


#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#define TRIGGER_PIN 3
#define ECHO_PIN 2
#define ADDR0_PIN 6			// Least significant of the address select
#define ADDR1_PIN 5
#define ADDR2_PIN 4			// Most significant of the address select bit

#define SENSOR_RIGHT_1		0
#define SENSOR_RIGHT_2		1
#define SENSOR_BACK_1		1
#define SENSOR_TOLERANCE	1
#define PARALLEL_TOLERANCE  1 
#define MAX_DISTANCE		244   //244 cm = 8 feet

#define LEFT	0
#define RIGHT	1
#define FRONT	1
#define BACK	0

#define NEAR	1
#define FAR		0

#define X_OFFSET 15
#define Y_OFFSET 5
#define STOP_DELAY 30
#define SPIN_PERIOD 2
#define CHECKPOINT_PERIOD 10
#define MOTOR_PERIOD 40


/*******************************************************************
*		Constant to tell the robot how many degrees to move by in order to make it self 
*		parallel to the wall. 
*
*		Pretend we are reading sensor 0 (A) and sensor 1 (B)
*		Difference (C) = A - B
*		Distance between A and B (D) = Y_OFFSET * 2
*
*		Degrees to move  = C / D
********************************************************************/
#define DEGREES_TO_MOVE 9
#define X_COUNTS_PER_CM 41
#define Y_COUNTS_PER_CM 32




class Ultrasonic
{
	//variables
	public:
	volatile unsigned long PreviousTime, InterruptEchoTime;
	volatile int Select, Edge;
	int EchoDistance[6];	// Array containing the time of the ultrasonic sensors
	int FullSet;
	int CheckPointFlag;
	int TargetFlag; 
	char DirectionFlag;		// 1 = Towards the target
							// 0 = Away from the target
	int XError;
	int YError; 
	int WError;
	
	int StateMachine;        // This is a State Machine that tells the robot what it should do
	// with the ultrasonic data
	// 0 -> The robot is trying to make it self parallel to the wall by rotating CW/CCW
	// 1 -> The robot is trying to reach the x-axis target by strafing
	// 2 -> The robot is trying to reach the y-axis target by moving forward or backward

	//functions
	public:
	Ultrasonic();
	~Ultrasonic();
	void interrupt();
	void initialize();
	void spinOnce(char Face);
	void trigger();
	void addressSelect(int Address);
	void pinSelect (int Address);
	void checkPoint(char Face, int XTartget, int YTarget);
	void forward();
	void turn90Cw();
	void stop();

	private:
	Ultrasonic( const Ultrasonic &c );
	Ultrasonic& operator=( const Ultrasonic &c );

}; //Ultrasonic

#endif //__ULTRASONIC_H__
