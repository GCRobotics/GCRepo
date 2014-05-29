/*
 * UltrasonicInterrupt.ino
 *
 * Created: 3/14/2014 7:22:23 PM
 * Author: Quang
 */ 

#define TRIGGER_PIN 3
#define ECHO_PIN 2
#define ADDR0_PIN 6			// Least significant of the address select
#define ADDR1_PIN 5
#define ADDR2_PIN 4			// Most significant of the address select bit

void ultrasonicInterrupt();
void ultrasonicInitialize();
void ultrasonicTrigger();
void ultrasonicAddressSelect(int Address);
void ultrasonicPinSelect (int Address);

volatile unsigned long PreviousTime = 0, InterruptEchoTime = 0;
volatile int ultrasonicSelect = 0, ultrasonicEdge = 0, ultrasonicCounter = 0;


	//unsigned long DebugPreviousTime = PreviousTime;
	//unsigned long DebugTime = EchoTime;
	
unsigned long EchoTime[6] = {0,0,0,0,0,0};		// Array containing the time of the ultrasonic sensors

void setup()
{
	ultrasonicInitialize();
}

void loop()
{
	ultrasonicSpinOnce();
}


void ultrasonicInitialize()
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
	
	// Attach int.0 interrupt at pin 2
	attachInterrupt(0,ultrasonicInterrupt,CHANGE);
}

void ultrasonicSpinOnce()
{
	if ((ultrasonicSelect == 0) || (ultrasonicSelect == 2) || (ultrasonicSelect == 4) ||
	(ultrasonicSelect == 6) || (ultrasonicSelect == 8) || (ultrasonicSelect == 10) || (ultrasonicSelect == 12))
	{
		// Record the ultrasonic sensor.
		// Skip the first one since there is nothing to read
		if (ultrasonicSelect > 0)
		{
			// The index of the array is equal to (ultrasonicSelect/2) +1
			// Ex.
			// We want to store of the first ultrasonic sensor, which is at ultrasonicSelect = 2
			// index = (2/2) - 1 = 0  <-- that's is the index that we want
			EchoTime[((ultrasonicSelect/2) - 1)] = InterruptEchoTime;
		}
		
		// Reset to zero at the value of 12
		if (ultrasonicSelect >= 12)
		{
			ultrasonicSelect = 0;
		}
		
		// Set the address pins to the correct address
		ultrasonicAddressSelect(ultrasonicSelect);
		// Set the trigger pin to HIGH for 10 us
		ultrasonicTrigger();
		// Increment to odd
		ultrasonicSelect++;
	}
}

void ultrasonicInterrupt()
{
	// If it just went from lOW to HIGH, then this is the beginning of the echo cycle
	if (digitalRead(ECHO_PIN) == 1) {
	//if (ultrasonicEdge == 0) {	
		PreviousTime = micros();
		ultrasonicEdge = 1;
	}
	// If it just went from HIGH to LOW, then this is the end of the echo cycle
	else if (digitalRead(ECHO_PIN) == 0){
	//else if (ultrasonicEdge == 1) {		
		InterruptEchoTime = micros() - PreviousTime;
		//Increment the select variable to even
		ultrasonicSelect++;
		ultrasonicEdge = 0;
	}
}

void ultrasonicTrigger()
{
	// According to the data sheet, we need to set trigger to high for 10 us
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER_PIN,LOW);
}

void ultrasonicAddressSelect(int Address)
{
	switch (Address)
	{
		case 0:
			ultrasonicPinSelect(0);
			break;
		case 2:
			ultrasonicPinSelect(1);
			break;
		case 4:
			ultrasonicPinSelect(2);
			break;
		case 6:
			ultrasonicPinSelect(3);
			break;
		case 8:
			ultrasonicPinSelect(4);
			break;
		case 10:
			ultrasonicPinSelect(5);
			break;
			
	}
}

void ultrasonicPinSelect (int Address)
{
	// Set the least sig. digit
	digitalWrite(ADDR0_PIN, (Address & 0x01));			// & is to mask the bits
	// Set the middle digit
	digitalWrite(ADDR1_PIN, (Address & 0x02));
	// Set the most sig. digit
	digitalWrite(ADDR2_PIN, (Address & 0x04));
}