/* 
* Motors.h
*
* Created: 3/16/2014 12:16:09 AM
* Author: Quang
*/


#ifndef __MOTORS_H__
#define __MOTORS_H__



#define Front_Left   0x08
#define Front_Right  0x02
#define Back_Right   0x04
#define Back_Left    0x06

#define Forward      0x00
#define Backward     0x01

#define SlowSpeed    20
#define MediumSpeed  50
#define FastSpeed    70
#define Stop         00


class Motors
{
//functions
public:
	Motors();
	~Motors();
	
	void forward(int RobotSpeed);
	void backward(int RobotSpeed);
	void left(int RobotSpeed);
	void right(int RobotSpeed);
	void cw(int RobotSpeed);
	void ccw(int RobotSpeed);
	void stop();
	void i2cWrite ( char Address, char Speed , char Direction);
	

}; //Motors

#endif //__MOTORS_H__
