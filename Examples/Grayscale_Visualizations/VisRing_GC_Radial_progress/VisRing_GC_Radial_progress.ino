/*
  Control a display extended smart ring
  Example: Radial progress chart
*/

#include <SPI.h>
#include <VisRing.h>

//Initialize the display with the follow pin connections
//Note that you should not change SCLK and MOSI because the
//library uses hardware SPI
VisRing VisRing(15, 16, 12, 13);  // NRF52832 15 = CS, 16 = RES, 13 = SCLK, 11 = SDIN
HardwareSerial* printer = &Serial;

void setup() {
  SPI.begin();
  Serial.begin(115200);
  VisRing.begin(160, 32, printer);  //Display is 160 wide, 32 high
  VisRing.clearDisplayGS();
}

void example_radialProgress(){
  for (int i = 0; i <= 8; i++) {
    VisRing.radialProgressChart(80, 15, 15, i, true, 1, 15, 12);
    VisRing.displayGS();
    delay(1000);
    VisRing.clearDisplayGS();
  }
}

void loop() {
  example_radialProgress();
}