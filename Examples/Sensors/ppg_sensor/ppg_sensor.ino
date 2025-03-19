/*
  VisRing code to read and display heart rate values.
*/

// oled
#include <VisRing.h>
#include <VisRingUtility.h>

VisRing VisRing(15, 16, 12, 13);

// int avgLength, int avgDetectLength, int turnOffPeriod, int delayPeriod
VisRingUtility VisRingUtility(600, 10, 1000, 300);

void displayHR() {
  if (VisRingUtility.dataPPG[0] != -1) {
    VisRing.clearDisplayGS();

    VisRing.printStringGS(
      10,
      12,
      "BPM:" + (String)(int)VisRingUtility.dataPPG[0] + "  AVG:" + (String)(int)VisRingUtility.dataPPG[1],
      false,
      15,
      0);

    VisRing.displayGS();
    print_data();
  }
}

void print_data() {
  Serial.println("BPM:" + (String)VisRingUtility.dataPPG[0] + "  AVG:" + (String)VisRingUtility.dataPPG[1]);
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  VisRingUtility.setupPPG();

  VisRing.begin(160, 32);
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
}

void loop() {
  VisRingUtility.updateDataPPG();
  displayHR();
}
