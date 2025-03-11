/*
  VisRing code to turn of screen after n seconds.
  Basic wakeup functionality with IMU accelerator: double tap or shaking. False positives happen.
*/

// General
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <movingAvg.h>  // https://github.com/JChristensen/movingAvg

// IMU
#include "ICM_20948.h"  // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
#define CS_PIN 2        // Which pin you connect CS to. Used only when "USE_SPI" is defined

// VisRing
#include <VisRing.h>

ICM_20948_SPI imu;  // If using SPI create an ICM_20948_SPI object

// Display setup
VisRing VisRing(15, 16, 12, 13);

// Rolling average for imu values
movingAvg avgTotalAcc(600, true);

// Timing variables
unsigned long startMillis;              // Start time of the program
unsigned long currentMillis;            // Used for current time
unsigned long millisOfLastTap = 0;      // Used to record time of first tap
const unsigned long delayPeriod = 300;  // Forced delay between first and second tap to ensure separate events
unsigned long displayStartTime = 0;     // Start time of display
unsigned long turnOffPeriod = 5000;      // Duration before display turns off

// Tap count
int count = 0;

// seconds since start to be displayed
int seconds = 0;

// Determines if display should be on
bool enableDisplay = true;

void imu_setup() {
  avgTotalAcc.begin();

  imu.enableDebugging();  // Uncomment this line to enable helpful debug messages on Serial

  bool initialized = false;
  while (!initialized) {

    imu.begin(CS_PIN, SPI);

    imu.startupDefault(false);  // Force a full - not minimal - startup
    Serial.print(F("Initialization of the sensor returned: "));
    Serial.println(imu.statusString());
    if (imu.status != ICM_20948_Stat_Ok) {
      Serial.println("Trying again...");
      delay(500);
    } else {
      initialized = true;
    }
  }
  Serial.println("ICM20948 Success!");
}

int get_imu_total_acc_int() {
  if (imu.dataReady()) {
    imu.getAGMT();
    return abs(imu.accX() * 0.01f + imu.accY() * 0.01f + imu.accZ() * 0.01f);
  } else {
    return -1;
  }
}

void turnOffDisplay() {
  currentMillis = millis();
  if (currentMillis > displayStartTime) {
    if (currentMillis - displayStartTime >= turnOffPeriod) {
      enableDisplay = false;
    }
  }
}

void detectTap() {
  currentMillis = millis();
  int acc = get_imu_total_acc_int();

  if (acc != -1) {
    int avgLong = avgTotalAcc.reading(acc);
    if (currentMillis - millisOfLastTap > delayPeriod) {
      int avgShort = avgTotalAcc.getAvg(10);
      if (avgShort > avgLong + 5) {
        count++;
        millisOfLastTap = millis();
        if (count > 1) {
          count = 0;
          enableDisplay = true;
          displayStartTime = millis();
        }
      }
    }
  }
}

void countSeconds() {
  currentMillis = millis();
  if (currentMillis - startMillis >= 1000) {
    seconds++;
    startMillis = millis();
  }
}

void displaySeconds() {
  VisRing.clearDisplayGS();
  VisRing.printStringGS(120, 10, (String)seconds, false, 15, 0);
  VisRing.displayGS();
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  imu_setup();

  startMillis = millis();  //save the start time

  VisRing.begin(160, 32);  // Display is 160 wide, 32 high
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
}

void loop() {
  countSeconds();

  detectTap();

  if (enableDisplay) {
    displaySeconds();
    turnOffDisplay();
  } else {
    VisRing.clearDisplayGS();
  }
}
