/*
  VisRing code to read and display temperature values.
*/
// oled
#include <VisRing.h>
#include <VisRingUtility.h>

VisRing VisRing(15, 16, 12, 13);

// int avgLength, int avgDetectLength, int turnOffPeriod, int delayPeriod
VisRingUtility VisRingUtility(600, 10, 1000, 300);

void displayTMP() {
  if (VisRingUtility.dataTemp != -1) {
    VisRing.clearDisplayGS();

    VisRing.printStringGS(
      10,
      12,
      "temp:" + (String)VisRingUtility.dataTemp,
      false,
      15,
      0);

    VisRing.displayGS();
    print_data();
  }
}

void print_data() {
  Serial.println("temp: " + (String)VisRingUtility.dataTemp);
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  VisRingUtility.setupTMP();

  VisRing.begin(160, 32);  // Display is 160 wide, 32 high
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
}

void loop() {
  VisRingUtility.updateDataTMP();
  displayTMP();
  delay(1000);
}
