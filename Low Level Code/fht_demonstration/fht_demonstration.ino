/*
fht_adc.pde
guest openmusiclabs.com 9.5.12
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht

#define LOWER_FREQUENCY 15
#define UPPER_FREQUENCY 25
#define LOWER_FREQUENCY_LIMIT 115
#define UPPER_FREQUENCY_LIMIT 128

int FhtCounter = 0;
int FhtAverageStackLowerFreq[8] = {3,4,5,6,7,8,9,10};
int FhtAverageLowerFreq = 0;
int FhtAverageStackUpperFreq[8] = {3,4,5,6,7,8,9,10};
int FhtAverageUpperFreq = 0;
  

int Shelf = 0;
          // 0 = Lower shelf
          // 1 = Upper shelf

#include <FHT.h> // include the library

void setup() {
  
  Serial.begin(115200); // use the serial port
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop() {
    /*******************************************************
  *
  *                FHT
  *
  ********************************************************/


  while(1)
  {
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fht_input[i] = k; // put real data into bins
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_log(); // take the output of the fht
    sei();
//    Serial.write(255); // send a start byte
//    Serial.write(fht_log_out, FHT_N/2); // send out the data
    
    // Collect the bin's value for averaging
    if (FhtCounter < 8)
    {
      FhtAverageStackLowerFreq[FhtCounter] = (int)fht_log_out[LOWER_FREQUENCY];
      FhtAverageStackUpperFreq[FhtCounter] = (int)fht_log_out[UPPER_FREQUENCY];
      FhtCounter++;
      
    }
    else 
    {
      // Add the frequency stack together
      for (int i = 0; i < 8; i++)
      {
        FhtAverageLowerFreq += FhtAverageStackLowerFreq[i];
        FhtAverageUpperFreq += FhtAverageStackUpperFreq[i];
      }
      // divide by 8 to get the average
      FhtAverageLowerFreq = FhtAverageLowerFreq >> 3;
      FhtAverageUpperFreq = FhtAverageUpperFreq >> 3;
      Serial.print("        Lower Freq Average is =   ");  Serial.println(FhtAverageLowerFreq);
      Serial.print("        Upper Freq Average is =   ");  Serial.println(FhtAverageUpperFreq); Serial.println();
      // if the average is greater than or equal to the upper limit
      // then we need to break off the infinite while loop
      if (FhtAverageLowerFreq >= LOWER_FREQUENCY_LIMIT)
      {
        // Indicating that the plate is on the lower shelf
        Shelf = 0;
        break;
      }
      // else we need to reset the counter and clear the average
      else if (FhtAverageUpperFreq >= UPPER_FREQUENCY_LIMIT)
      {
        // Indicating that the plate is on the upper shelf
        Shelf = 1;
        break;
      }
      else {
        FhtCounter = 0;
        FhtAverageLowerFreq = 0;
        FhtAverageUpperFreq = 0;
      }
    
    }
//    delay(100);
  }

  Serial.println("                        Value obtained!!");
  while(1)
  {
    Serial.print("Shelf is =  "); Serial.println(Shelf);
    if (Shelf == 1)
      digitalWrite(11, HIGH);
    else
      digitalWrite(12, HIGH);
  }
}

