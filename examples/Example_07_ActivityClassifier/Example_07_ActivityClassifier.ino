/*
  Using the BNO08x IMU

  This is a fun one! The BNO080 can guess at what activity you are doing:
  In vehicle
  On bicycle
  On foot
  Still
  Tilting
  Walking
  Running
  On stairs
  This example shows how to read the confidence levels of each activity

  By: Nathan Seidle
  SparkFun Electronics
  Date: December 21st, 2017
  SparkFun code, firmware, and software is released under the MIT License.
	Please see LICENSE.md for further details.

  Originally written by Nathan Seidle @ SparkFun Electronics, December 28th, 2017

  Adjusted by Pete Lewis @ SparkFun Electronics, June 2023 to incorporate the
  CEVA Sensor Hub Driver, found here:
  https://github.com/ceva-dsp/sh2

  Also, utilizing code from the Adafruit BNO08x Arduino Library by Bryan Siepert
  for Adafruit Industries. Found here:
  https://github.com/adafruit/Adafruit_BNO08x

  Also, utilizing I2C and SPI read/write functions and code from the Adafruit
  BusIO library found here:
  https://github.com/adafruit/Adafruit_BusIO

  Hardware Connections:
  Plug the sensor into IoT RedBoard via QWIIC cable.
  Serial.print it out at 115200 baud to serial monitor.

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/22857
*/

#include <Wire.h>

#include "SparkFun_BNO08x_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO08x
BNO08x myIMU;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO08x Read Example");

  Wire.begin();

  if (myIMU.begin() == false) {
    Serial.println("BNO08x not detected at default I2C address. Check your jumpers and the hookup guide. Freezing...");
    while (1)
      ;
  }
  Serial.println("BNO08x found!");

  // Wire.setClock(400000); //Increase I2C data rate to 400kHz

  setReports();

  Serial.println("Reading events");
  delay(100);
}

// Here is where you define the sensor outputs you want to receive
void setReports(void) {

  Serial.println("Setting desired reports");

  //See page 73 of reference manual. There is a 32 bit word where each
  //bit enables a different possible activity. Currently there are only 8
  //possible. Let's enable all of them!
  uint32_t enableActivities = 0x1F; //Enable all 9 possible activities including Unknown

  //Send data update every 1000ms, with sensor specific config word

  if (myIMU.enableActivityClassifier(1000, enableActivities) == true) {
    Serial.println(F("Activity Classifier enabled"));
  } else {
    Serial.println("Could not enable Activity Classifier");
  }
}

void loop() {
  delay(1000); // Personal Activity Classifier needs longer delay here than other examples

  if (myIMU.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }

  // Has a new event come in on the Sensor Hub Bus?
  if (myIMU.getSensorEvent() == true) {

    // is it the correct sensor data we want?
    if (myIMU.getSensorEventID() == SENSOR_REPORTID_PERSONAL_ACTIVITY_CLASSIFIER) {

    byte mostLikelyActivity = myIMU.getActivityClassifier();

    Serial.print("Most likely activity: ");
    printActivityName(mostLikelyActivity);
    Serial.println();

    Serial.println("Confidence levels:");
    for(int x = 0 ; x < 9 ; x++)
      {
        printActivityName(x);
        Serial.print(F(") "));
        Serial.print(myIMU.getActivityConfidence(x));
        Serial.print(F("%"));
        Serial.println();
      }

    Serial.println();
    }
  }
}

//Given a number between 0 and 8, print the name of the activity
//See page 73 of reference manual for activity list
void printActivityName(byte activityNumber)
{
  if(activityNumber == 0) Serial.print("Unknown");
  else if(activityNumber == 1) Serial.print("In vehicle");
  else if(activityNumber == 2) Serial.print("On bicycle");
  else if(activityNumber == 3) Serial.print("On foot");
  else if(activityNumber == 4) Serial.print("Still");
  else if(activityNumber == 5) Serial.print("Tilting");
  else if(activityNumber == 6) Serial.print("Walking");
  else if(activityNumber == 7) Serial.print("Running");
  else if(activityNumber == 8) Serial.print("On stairs");
}
