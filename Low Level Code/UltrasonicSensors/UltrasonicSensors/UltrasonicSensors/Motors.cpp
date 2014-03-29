/* 
* Motors.cpp
*
* Created: 3/16/2014 12:16:09 AM
* Author: Quang
*/


#include "Motors.h"
#include <Wire.h>
#include <Arduino.h>

// default constructor
Motors::Motors()
{
	PreviousTime = 0;
} //Motors

// default destructor
Motors::~Motors()
{
} //~Motors

void Motors::forward(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Forward  );
	i2cWrite(Front_Right,  RobotSpeed,   Forward  );
	i2cWrite(Back_Left,    RobotSpeed,   Forward  );
	i2cWrite(Back_Right,   RobotSpeed,   Forward  );
}

void Motors::backward(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Backward  );
	i2cWrite(Front_Right,  RobotSpeed,   Backward  );
	i2cWrite(Back_Left,    RobotSpeed,   Backward  );
	i2cWrite(Back_Right,   RobotSpeed,   Backward  );
}

void Motors::left(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Backward  );
	i2cWrite(Front_Right,  RobotSpeed,   Forward   );
	i2cWrite(Back_Left,    RobotSpeed,   Forward   );
	i2cWrite(Back_Right,   RobotSpeed,   Backward  );
}

void Motors::right(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Forward   );
	i2cWrite(Front_Right,  RobotSpeed,   Backward  );
	i2cWrite(Back_Left,    RobotSpeed,   Backward  );
	i2cWrite(Back_Right,   RobotSpeed,   Forward   );
}

void Motors::cw(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Forward   );
	i2cWrite(Front_Right,  RobotSpeed,   Backward  );
	i2cWrite(Back_Left,    RobotSpeed,   Forward   );
	i2cWrite(Back_Right,   RobotSpeed,   Backward  );
}

void Motors::ccw(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Backward   );
	i2cWrite(Front_Right,  RobotSpeed,   Forward    );
	i2cWrite(Back_Left,    RobotSpeed,   Backward   );
	i2cWrite(Back_Right,   RobotSpeed,   Forward    );
}

void Motors::stop()
{
	i2cWrite(Front_Left,   Stop,   Forward   );
	i2cWrite(Front_Right,  Stop,   Forward  );
	i2cWrite(Back_Left,    Stop,   Forward  );
	i2cWrite(Back_Right,   Stop,   Forward   );
}

void Motors::i2cWrite ( char Address, char Speed , char Direction)
{
	Wire.beginTransmission(Address >> 1 );
	Wire.write((byte)0);
	Wire.write((byte)Speed);
	Wire.write((byte)Direction);
	Wire.endTransmission();
}

int Motors::moveForward( int *Target)
{
	if ( (millis() - PreviousTime) >= SAMPLING_PERIOD)
	{
		forward(MediumSpeed);
		
		float DistanceTraveled = 0.0;
		
		//Retrieve Odometry and Calculate average
		DistanceTraveled = average();
		
		// Calculate distance travel
		DistanceTraveled = DistanceTraveled * Y_MM_DISTANCE_PER_COUNT;
		
		PreviousTime = millis();
		(*Target) -= DistanceTraveled;
		
		// If target is reached
		if ((*Target) <= 0)
		{
			stop();
			return 1;
		}
		else return 0;
		
	}
}

float Motors::average()
{
	int Encoder[4] = {0,0,0,0};
	Encoder[0] = readOne(Front_Right);
	Encoder[1] = readOne(Back_Right);
	Encoder[2] = readOne(Back_Left);
	Encoder[3] = readOne(Front_Left);
	
	float AverageResult = 0;
	for (int i=0 ; i < 4; i++)
	{
		AverageResult += (float)(Encoder[i]);
	}
	AverageResult = AverageResult/4.0;
	return AverageResult;
}

int Motors::readOne(char address) 
{                               // pass in the motor you want to read
	unsigned int encoder[2] = {0,0};
	Wire.requestFrom(address >> 1 , 2);    // request 2 bytes from address
	int i = 0;
	while(Wire.available())   // slave may send less than requested
	{
		encoder[i] = Wire.read();   // receive a byte as character
		i++;
	}
	encoder[1] = encoder[1] << 8; // Combine the two bytes into one value, lower byte is sent first, upper second.
	
	return encoder[1] | encoder[0];
}