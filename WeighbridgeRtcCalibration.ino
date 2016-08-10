// Penguin Weighbridge
// Real-Time Clock Calibration

// Created by Ben Pitcher (ben.pitcher@mq.edu.au)
// Originally created by adafruit (https://learn.adafruit.com/adafruit-data-logger-shield/overview)

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

// This sketch sets the real-time clock (RTC) on the data logging shiled.
// The time set on the RTC is taken from the computer used to compile the sketch.
// The time that is uploaded to the RTC is taken at the time the sketch is compiled. It is therefore essential that the sketch is compiled and uploaded immediatley.
// There will be a slight lag between the computer time and the RTC time due to the delay in compiling and uploading the sketch.

// Make a note of the timezone (e.g. UTC+10) that is being used on the computer compiling the sketch. This is needed in the main Weighbridge sketch to correct the time to UTC.

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

void setup () {
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // uncomment it & upload to set the time, date and start run the RTC!
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

}

void loop () {
    DateTime now = RTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" since 1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    Serial.println();
    delay(3000);
}
