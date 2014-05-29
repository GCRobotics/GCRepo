/*
 * Master.ino
 *
 * Created: 4/1/2014 3:00:54 PM
 * Author: Quang
 */ 
#include "Arduino.h"
#include "Lift.h"

Lift Master;
int SlaveReturn = 0;
int Temp1 = 0, Temp2 = 0, Temp3 = 0;

void setup()
{
	Master.MasterInitialize();
	Master.MasterSetLevel(1);
	Master.MasterRequestStateChange();
}

void loop()
{
	SlaveReturn = Master.MasterSpinOnce();

}
