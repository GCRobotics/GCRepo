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
#define SENSOR_BACK_1		2
#define SENSOR_TOLERANCE	2
#define MAX_DISTANCE		200

#define LEFT	0
#define RIGHT	1
#define FRONT	1
#define BACK	0

#define NEAR	1
#define FAR		0



class Ultrasonic
{
	//variables
	public:
	volatile unsigned long PreviousTime, InterruptEchoTime;
	volatile int Select, Edge;
	int EchoDistance[6];	// Array containing the time of the ultrasonic sensors
	int FullSet;
	int CheckPointFlag;
	
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
	void spinOnce();
	void trigger();
	void addressSelect(int Address);
	void pinSelect (int Address);
	void checkPoint(char Side, char Face, int XTartget, int YTarget, int Near);
	void forward();
	void turn90Cw();
	void stop();

	private:
	Ultrasonic( const Ultrasonic &c );
	Ultrasonic& operator=( const Ultrasonic &c );

}; //Ultrasonic

#endif //__ULTRASONIC_H__
