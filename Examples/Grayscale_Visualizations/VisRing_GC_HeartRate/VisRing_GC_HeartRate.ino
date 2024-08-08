/*
  Control a display extended smart ring
  Example: Heart Rate 1
*/

#include <SPI.h>
#include <VisRing.h>

// Initialize the display with the follow pin connections
// Note that you should not change SCLK and MOSI because the
// library uses hardware SPI
VisRing VisRing(15, 16, 12, 13);
HardwareSerial *printer = &Serial;

void setup() {
  SPI.begin();
  Serial.begin(115200);
  VisRing.begin(160, 32, printer);  // Display is 160 wide, 32 high
  VisRing.clearDisplayGS();
}

void example_HearRate() {
  int hr[] = { 50, 55, 60, 60, 65, 70, 70, 90, 100, 100, 90, 70, 50, 50 };

  for (int variant = 0; variant < 2; variant++) {
    for (int val : hr) {
      VisRing.drawHR(val, variant, 15, 4);
      VisRing.displayGS();
      delay(500);
      VisRing.clearDisplayGS();
    }
  }
}

void loop() {
  example_HearRate();
}