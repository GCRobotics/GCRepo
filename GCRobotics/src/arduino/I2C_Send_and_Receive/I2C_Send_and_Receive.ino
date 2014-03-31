/*

		NAME 	 - Arduino Control Bridge 
		DESIGN - Josh Galicic
		PRIORITY - Required - Neccessary for communicating between PC and Pics.
	
		--------OVERVIEW--------
This code allow the arduino to subscribe to the i2cSend topic (as well as others) and send these
i2c requests to the specified pics. The i2c message type allows for 2 bytes of data and an address.
Filling these fields out will let the arduino handle the rest in the i2c callback.

This code also allows the arduino to request encoder data from the pics and send it back up on the
/encoderData topic. This happens on a timer1 interrupt (line 80) currently set to 2 seconds.
There is also code and hardware for the arduino to read the battery voltage and send it back to insure
that the battery is not drained too low.


		--------FUTURE WORK--------
The battery voltage code is not currently implemented, but it has been verified as working correctly.
It is connected to a voltage divider on the regulator board to limit it to the 5 volt range. Refine
the conversion factor from analog input to battery voltage, because it isnt super accurate, maybe
look into a better voltage and/or current sensor to can provide more usable data.

Sending code has been verified as stable, and should give no issues, the pin 13 led will go high
whenever the arduino is sending or receiving i2c data. If you every see that light being constantly on,
it means that something on the bus is probably holding the clk line low, blocking all communication.
This can happen when reprogramming the PICs, so probably just reset power to the boards and hit the reset
button on the arduino. the serial bridge will survive an arduino reset, so that can keep going without
issue. 

*/

#include <ArduinoHardware.h>
#include <ros.h>
#include <Wire.h>
#include <GCRobotics/Encoder_msg.h>
#include <GCRobotics/i2cData.h>
#include <GCRobotics/command_state.h>
#include <std_msgs/Float32.h>
#include <std_msgs/UInt16.h>
#include <TimerOne.h>

void i2cCallback( const GCRobotics::i2cData& msg);
void gpioCallback( const std_msgs::UInt16& msg);
void command_callback(const GCRobotics::command_state& msg);
void Read();
void ReadOne(char,float*,bool*);

ros::NodeHandle n;

//Message data variables
GCRobotics::Encoder_msg encoders;
int ros_state;
int arduino_state;
GCRobotics::command_state _state;
std_msgs::Float32 voltage;
std_msgs::UInt16 errorCode;
//pubs
ros::Publisher encoderPub("EncoderData", &encoders);
ros::Publisher command_pub("/command_state", &_state);
ros::Publisher diagPub("BatteryVoltage", &voltage);
ros::Publisher errorPub("ArduinoError", &errorCode);
// subs
ros::Subscriber<GCRobotics::command_state> command_sub("command_state", &command_callback);
ros::Subscriber<GCRobotics::i2cData> i2cSub("i2cSend", &i2cCallback );
ros::Subscriber<std_msgs::UInt16> gpioSub("gpio", &gpioCallback);

void setup(){
  Wire.begin(); // join i2c bus
  n.initNode();
  n.advertise(encoderPub);
  n.advertise(command_pub);
  n.advertise(diagPub);
  n.advertise(errorPub);
  
  n.subscribe(command_sub);
  n.subscribe(i2cSub);
  n.subscribe(gpioSub);

  pinMode(13, OUTPUT);
  //DDRD = B11111111;
  
  errorCode.data = 0;
  Timer1.initialize(100000); // 100 ms between interrupts
  Timer1.attachInterrupt(Read);
 // Timer1.attachInterrupt(sendVoltage);

}

void loop(){
    n.spinOnce();
    
    switch (arduino_state)
    {
        case 0:
            digitalWrite(13, HIGH);
            delay(5000);
            _state.state = ++arduino_state;
            command_pub.publish(&_state);
            break;
        case 2: 
            digitalWrite(13, HIGH);
            delay(5000);
            _state.state = ++arduino_state;
            command_pub.publish(&_state);
            break;
        case 4: 
            digitalWrite(13, HIGH);
            delay(5000);
            _state.state = ++arduino_state;
            command_pub.publish(&_state);
            break;
        case 6: 
            digitalWrite(13, HIGH);
            delay(5000);
            _state.state = ++arduino_state;
            command_pub.publish(&_state);
            break;
        case 8: 
            digitalWrite(13, HIGH);
            delay(5000);
            _state.state = ++arduino_state;
            command_pub.publish(&_state);
            break;
        case 10: 
            digitalWrite(13, HIGH);
            delay(5000);
            _state.state = ++arduino_state;
            command_pub.publish(&_state);
            break;
        case 12: 
            digitalWrite(13, HIGH);
            delay(5000);
            _state.state = ++arduino_state;
            command_pub.publish(&_state);
            break;
        default:
            arduino_state = ros_state;
    }


  
  /*
  TRANSMIT TESTING CODE
  Wire.beginTransmission(0x04>>1); // transmit to device "address"
  Wire.write((byte)0);
  Wire.write((byte)200);
  Wire.write((byte)1);
  Wire.endTransmission();  
    digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  */
  
  
}

void command_callback(const GCRobotics::command_state& msg)
{
    ros_state = msg.state;
}

void i2cCallback( const GCRobotics::i2cData& msg)
{

  digitalWrite(13,HIGH);
  Wire.beginTransmission(msg.address>>1); // transmit to device "address"
  Wire.write((byte)0);
  Wire.write(msg.messageData);
  Wire.write(msg.messageData2);
  errorCode.data = Wire.endTransmission(); 
  errorCode.data = msg.address;
  errorPub.publish(&errorCode);
 
  digitalWrite(13,LOW);
}

void gpioCallback( const std_msgs::UInt16& msg)
{
  PORTD = msg.data;
}

void Read() {
  sei(); // enable interrupts inside of this interrrupt, allowing wire function calls to still function instead of blocking, fixed your n00bish ways
  digitalWrite(13,HIGH);
  ReadOne(0x04, &encoders.encoder1, &encoders.direction1);
  ReadOne(0x02, &encoders.encoder2, &encoders.direction2);
  ReadOne(0x06, &encoders.encoder3, &encoders.direction3);
  ReadOne(0x08, &encoders.encoder4, &encoders.direction4);
  
  //Publish the results
  encoderPub.publish(&encoders);
  digitalWrite(13,LOW);

}

void ReadOne(char address, float *Odometry, bool *Direction) {                               // pass in the motor you want to read
  unsigned int encoder[4] = {0,0,0,0};
  Wire.requestFrom(address >>1 , 4);    // request 3 bytes from address
   int i = 0;
  while(Wire.available())   // slave may send less than requested
  { 
    encoder[i] = Wire.read();   // receive a byte as character
    i++;
  }
  
  encoder[1] = encoder[1] << 8; // Combine the two bytes into one value, lower byte is sent first, upper second.
  *Odometry = encoder[1] + encoder[0];
  
  encoder[3] = encoder[3] << 8; // Combine the two bytes into one value, lower byte is sent first, upper second.
  if( encoder[3] + encoder[2]==1)
    *Direction = true;
  else
    *Direction = false;
}

void sendVoltage()
{
   voltage.data  = analogRead(0); // average reading
  voltage.data += analogRead(0);
  voltage.data += analogRead(0);
  voltage.data += analogRead(0);
  voltage.data = voltage.data/4;
  
  voltage.data = voltage.data * 0.021251222; // analog/1023 * 5 * 4.348, convert to voltage
  //delay(200);
  diagPub.publish(&voltage);
}

