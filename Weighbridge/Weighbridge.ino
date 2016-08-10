// Penguin Weighbridge
// Weighbridge Operation Sketch

// Created by Ben Pitcher (ben.pitcher@mq.edu.au)
// Released under an MIT license (http://opensource.org/licenses/MIT).



// Libraries (Ensure all libraries are available prior to compiling sketch):
#include <SoftwareSerial.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>



// Configuration (Set these values to :
#define ZERO_OFFSET            13.2       // Zero offset value found from the calibration sketch.

#define GRAMS_PER_MEASUREMENT  6.73401    // Grams per measurement value found from the calibration sketch.

#define rep                    1000       // Set the number of mass samples to be taken for each chip read.

#define wait                   1000       // Set the delay in milliseconds to wait after reading a tag before listening for another tag.

#define ADC_PIN                0          // Set this to the analog input which is connected to the instrument amp output (if using the RobotShop Load Cell Amplifier Shield Strain1 = AnalogIn 0, Strain2 = AnalogIn 1).

#define tag                    16         // Set the number of characters in the RFID tag number (normally 16 characters for FDX-B/HDX animal identification transponder Output where the code is a 3 digit country code and a 12 digit ID seperated by an underscore).



// Internal state used by the sketch (Do not alter):
RTC_DS1307 rtc;
SoftwareSerial RFID(2, 3); // RFID RX and TX (note: this is RX and TX on the Arduino side. RX on the Arduino connects to TX on the RFIDRW-E-TTL unit and vice versa).
char c;
const int chipSelect = 10;
File logfile;
float sample = 0.0;



void setup()
{
  RFID.begin(9600); // Start serial to RFID reader
    
  Wire.begin(); // Start communication with the RTC
  
  rtc.begin();
  
  // make sure that the default chip select pin is set to output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  SD.begin(chipSelect);
  
  // Setup logfile.csv on SD card. If logfile.csv already exists a new row of headers will be added to the file.
  // A time-stamped row marked "BOOT" will be printed under the headers indicating when the system was powered up.
  
  logfile = SD.open("logfile.csv", FILE_WRITE);
  
  DateTime now = rtc.now();

  logfile.print("TIME, ");
  logfile.print("ID, ");
  for (int i = 1; i <= rep; i++)
  {
    logfile.print("MASS_");
    logfile.print(i);
    logfile.print(", ");
  }
  logfile.println();
  logfile.print(now.unixtime());
  logfile.print(", BOOT");
  for (int i = 0; i <= rep; i++)
  {logfile.print(", ");
  }
  logfile.println();
  
  logfile.close();

}


void loop()
{

  // The system now sits listening for RFID chips to come into range. When a chip is detected the ID number and time are recorded. The system then reads and records the mass from the loadcell the specified number of times before waiting a specified period to begin listening for chips again.
  
  DateTime now = rtc.now();

  if (RFID.available() > 0)
  {

    // Print information to SD Card
    logfile = SD.open("logfile.csv", FILE_WRITE);
    logfile.print(now.unixtime());
    logfile.print(", ");

    for (int i = 0; i <= tag; i++) 
    {
      c = RFID.read();
      // if it was a carriage return send a space, otherwise print the character
    if (c == '\r') { logfile.print(' ');
    }
     else { logfile.print(c);
     }
    }

    logfile.print(", ");

    for (int i = 1; i <= rep; i++) 
    {
      sample = analogRead(ADC_PIN) - ZERO_OFFSET;
      logfile.print(sample * GRAMS_PER_MEASUREMENT);
      logfile.print(", ");
    }

    logfile.println();
    logfile.close();

    delay(wait); 
  }

}
