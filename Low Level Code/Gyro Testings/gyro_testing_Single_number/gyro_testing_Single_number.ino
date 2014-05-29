#include <Arduino.h> // for byte data type
#include <Wire.h>
#include <math.h>

#define NUMBER_OF_SETS  32
#define NUMBER_OF_BYTES  6

// Debugging Mode ON/OFF Selector
// Change the constant to 0 if you don't want the controller to give back values using serial print
#define DEBUGGING_MODE  0
// device types
#define L3G_DEVICE_AUTO 0
#define L3G4200D_DEVICE 1
#define L3GD20_DEVICE   2

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define FIFO_CTRL_REG  0x2E
#define FIFO_SRC_REG   0x2F


// SA0 states

#define L3G_SA0_LOW  0
#define L3G_SA0_HIGH 1
#define L3G_SA0_AUTO 2


// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define L3G4200D_ADDRESS_SA0_LOW  (0xD0 >> 1)
#define L3G4200D_ADDRESS_SA0_HIGH (0xD2 >> 1)
#define L3GD20_ADDRESS_SA0_LOW    (0xD4 >> 1)
#define L3GD20_ADDRESS_SA0_HIGH   (0xD6 >> 1)

// register addresses

#define L3G_WHO_AM_I      0x0F

#define L3G_CTRL_REG1     0x20
#define L3G_CTRL_REG2     0x21
#define L3G_CTRL_REG3     0x22
#define L3G_CTRL_REG4     0x23
#define L3G_CTRL_REG5     0x24
#define L3G_REFERENCE     0x25
#define L3G_OUT_TEMP      0x26
#define L3G_STATUS_REG    0x27

#define L3G_OUT_X_L       0x28
#define L3G_OUT_X_H       0x29
#define L3G_OUT_Y_L       0x2A
#define L3G_OUT_Y_H       0x2B
#define L3G_OUT_Z_L       0x2C
#define L3G_OUT_Z_H       0x2D

#define L3G_FIFO_CTRL_REG 0x2E
#define L3G_FIFO_SRC_REG  0x2F

#define L3G_INT1_CFG      0x30
#define L3G_INT1_SRC      0x31
#define L3G_INT1_THS_XH   0x32
#define L3G_INT1_THS_XL   0x33
#define L3G_INT1_THS_YH   0x34
#define L3G_INT1_THS_YL   0x35
#define L3G_INT1_THS_ZH   0x36
#define L3G_INT1_THS_ZL   0x37
#define L3G_INT1_DURATION 0x38
#define L3G_LOW_ODR      0x39

class L3G
{
  public:
    typedef struct vector
    {
      volatile float x[NUMBER_OF_SETS], y[NUMBER_OF_SETS], z[NUMBER_OF_SETS]; 
      float CurrentAngle_x, CurrentAngle_y, CurrentAngle_z;
      volatile int ReadyToProcessFlag;
    } vector;
    vector g; // gyro angular velocity readings

    bool init(byte device = L3G_DEVICE_AUTO, byte sa0 = L3G_SA0_AUTO);

    void enableDefault(void);

    void writeReg(byte reg, byte value);
    byte readReg(byte reg);

    void read(void);

  private:
      byte _device; // chip type (4200D or D20)
      byte address;

      bool autoDetectAddress(void);
};

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define L3G4200D_ADDRESS_SA0_LOW  (0xD0 >> 1)
#define L3G4200D_ADDRESS_SA0_HIGH (0xD2 >> 1)
#define L3GD20_ADDRESS_SA0_LOW    (0xD4 >> 1)
#define L3GD20_ADDRESS_SA0_HIGH   (0xD6 >> 1)

// Public Methods //////////////////////////////////////////////////////////////

bool L3G::init(byte device, byte sa0)
{
  _device = device;
  switch (_device)
  {
    case L3G4200D_DEVICE:
      if (sa0 == L3G_SA0_LOW)
      {
        address = L3G4200D_ADDRESS_SA0_LOW;
        return true;
      }
      else if (sa0 == L3G_SA0_HIGH)
      {
        address = L3G4200D_ADDRESS_SA0_HIGH;
        return true;
      }
      else
        return autoDetectAddress();
      break;

    case L3GD20_DEVICE:
      if (sa0 == L3G_SA0_LOW)
      {
        address = L3GD20_ADDRESS_SA0_LOW;
        return true;
      }
      else if (sa0 == L3G_SA0_HIGH)
      {
        address = L3GD20_ADDRESS_SA0_HIGH;
        return true;
      }
      else
        return autoDetectAddress();
      break;

    default:
      return autoDetectAddress();
  }
}

// Turns on the L3G's gyro and places it in normal mode.
void L3G::enableDefault(void)
{
  // 0x0F = 0b00001111
  // Normal power mode, all axes enabled
  if (DEBUGGING_MODE)
  Serial.println("                Writing 0x0F to L3G_CTRL_REG1");
  writeReg(L3G_CTRL_REG1, 0x0F);
  // Enable OVR interrupt on I2
  if (DEBUGGING_MODE)
  Serial.println("                Writing 0x02 to L3G_CTRL_REG3");
  writeReg(L3G_CTRL_REG3, 0x02);
  // Enable FIFO_EN 
  if (DEBUGGING_MODE)
  Serial.println("                Setting FIFO_EN by writing 0x40 to L3G_CTRL_REG5");
  writeReg(L3G_CTRL_REG5, 0x40);
  // Turn on device in STREAMING MODE
  if (DEBUGGING_MODE)
  Serial.println("                Start device in Sreaming mode by writing 0xE0 to L3G_FIFO_CTRL_REG");
  writeReg(L3G_FIFO_CTRL_REG, 0x40);
  
  // Initialize all x,y, and z variables for L3G.g
  Serial.println("                Initializing g.__");
  for (int i =0; i < NUMBER_OF_SETS; i++)
  {
	g.x[i] = 0;
	g.y[i] = 0;
	g.z[i] = 0;
  }
  Serial.println("                Initializing g.CurrentAngle___");
  // Initialize CurrentAngle
  g.CurrentAngle_x = 0;
  g.CurrentAngle_y = 0;
  g.CurrentAngle_z = 0;
  
  Serial.println("                Initializing g.ReadyToProcessFlag");
  // Initialize ReadyToProcessFlag
  g.ReadyToProcessFlag = 0;
}

// Writes a gyro register
void L3G::writeReg(byte reg, byte value)
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// Reads a gyro register
byte L3G::readReg(byte reg)
{
  byte value;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(address, (byte)1);
  value = Wire.read();
  Wire.endTransmission();

  return value;
}

// Reads the 3 gyro channels and stores them in vector g
void L3G::read()
{
  Wire.beginTransmission(address);
  Wire.endTransmission();

  for (int i = 0; i <  NUMBER_OF_SETS; i++)            // Stuff the 192 bytes into g.x, g.y, and g.z
  {
  // assert the MSB of the address to get the gyro
  // to do slave-transmit subaddress updating.
  Wire.write(L3G_OUT_X_L | (1 << 7));
  Wire.requestFrom(address, (byte)NUMBER_OF_BYTES);
  while (Wire.available() < NUMBER_OF_BYTES);
  uint8_t xlg = Wire.read();
  uint8_t xhg = Wire.read();
  uint8_t ylg = Wire.read();
  uint8_t yhg = Wire.read();
  uint8_t zlg = Wire.read();
  uint8_t zhg = Wire.read();

  // combine high and low bytes
  g.x[i] = (int16_t)(xhg << 8 | xlg);
  g.y[i] = (int16_t)(yhg << 8 | ylg);
  g.z[i] = (int16_t)(zhg << 8 | zlg);
  }
  
  if (DEBUGGING_MODE)
  {
  // Print first (0), in-between (96), and last (191) data points to the screen
  Serial.println("                Printing the first, middle, and last data point that was just read        ");
  for (int i = 0; i < NUMBER_OF_BYTES; i = i + (NUMBER_OF_BYTES/2))
  {
    Serial.print ("g.x["); Serial.print (i); Serial.print("] : "); Serial.print(g.x[i]); Serial.print("        ");
    Serial.print ("g.y["); Serial.print (i); Serial.print("] : "); Serial.print(g.y[i]); Serial.print("        ");
    Serial.print ("g.z["); Serial.print (i); Serial.print("] : "); Serial.print(g.z[i]); Serial.println("");
  }
  }
  
  //Toggle ReadyToProcessFlag to ON
  g.ReadyToProcessFlag = 1;
}


// Private Methods //////////////////////////////////////////////////////////////

bool L3G::autoDetectAddress(void)
{
  // try each possible address and stop if reading WHO_AM_I returns the expected response
  address = L3G4200D_ADDRESS_SA0_LOW;
  if (readReg(L3G_WHO_AM_I) == 0xD3) return true;
  address = L3G4200D_ADDRESS_SA0_HIGH;
  if (readReg(L3G_WHO_AM_I) == 0xD3) return true;
  address = L3GD20_ADDRESS_SA0_LOW;
  if (readReg(L3G_WHO_AM_I) == 0xD4 || readReg(L3G_WHO_AM_I) == 0xD7) return true;
  address = L3GD20_ADDRESS_SA0_HIGH;
  if (readReg(L3G_WHO_AM_I) == 0xD4 || readReg(L3G_WHO_AM_I) == 0xD7) return true;

  return false;
}


/**************************************************************************************
*
*                        The real fun starts here!!
*
**************************************************************************************/
L3G gyro;

void setup() {
  Serial.begin(9600);
  
  Serial.println("                Setting up ISR on pin2");
  
  Wire.begin();

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }
  Serial.println("                Configuring Gyroscope");
  gyro.enableDefault();
  
  Serial.println("                Configuring Digital Pin 2 as INPUT");
  pinMode(2, INPUT);
  
  
  Serial.print("CTRL_REG1:      "); Serial.println(gyro.readReg(CTRL_REG1),HEX);
  Serial.print("CTRL_REG2:      "); Serial.println(gyro.readReg(CTRL_REG2),HEX);
  Serial.print("CTRL_REG3:      "); Serial.println(gyro.readReg(CTRL_REG3),HEX);
  Serial.print("CTRL_REG4:      "); Serial.println(gyro.readReg(CTRL_REG4),HEX);
  Serial.print("CTRL_REG5:      "); Serial.println(gyro.readReg(CTRL_REG5),HEX);
  Serial.print("FIFO_CTRL_REG:  "); Serial.println(gyro.readReg(FIFO_CTRL_REG),HEX);
  Serial.print("FIFO_SRC_REG:   "); Serial.println(gyro.readReg(FIFO_SRC_REG),HEX);
  
}

void loop() {
  unsigned long PreviousTime = micros();
  if (digitalRead(2))
  {
    unsigned long TimePassed = micros() - PreviousTime;
//    Serial.print("                  Reading Gyroscope!   "), Serial.print(TimePassed), Serial.println(" us has passed");
    gyro.read();
  }
}

/********************************************************************************
*
*                  Gyroscope ISR
*         This will kicks in whenever the FIFO of the gyro is full. Well...hopefully
*
*********************************************************************************/

