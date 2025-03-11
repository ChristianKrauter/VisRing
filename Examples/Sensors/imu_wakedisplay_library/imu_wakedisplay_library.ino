/*
  VisRing code to turn of screen after n seconds.
  Basic wakeup functionality with IMU accelerator: double tap or shaking. False positives happen.
*/

#include <VisRing.h>
#include <VisRingUtility.h>

VisRing VisRing(15, 16, 12, 13);
VisRingUtility VisRingUtility(600, 10, 5000, 300);

// seconds since start to be displayed
int seconds = 0;

void countSeconds() {
  VisRingUtility.currentMillis = millis();
  if (VisRingUtility.currentMillis - VisRingUtility.startMillis >= 1000) {
    seconds++;
    VisRingUtility.startMillis = millis();
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

  VisRingUtility.startMillis = millis();  //save the start time

  VisRingUtility.setupIMU();

  VisRing.begin(160, 32);  // Display is 160 wide, 32 high
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
}

void loop() {
  countSeconds();

  VisRingUtility.detectTap();
  
  if (VisRingUtility.enableDisplay) {
    displaySeconds();
    VisRingUtility.turnOffDisplay();
  } else {
    VisRing.clearDisplayGS();
  }
}
