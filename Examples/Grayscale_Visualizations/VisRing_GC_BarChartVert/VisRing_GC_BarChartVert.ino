/*
  Control a display extended smart ring
  Example: Bar Chart for when the ring is at vertical orientation
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

void example_BarChartVert(){
  int values_count = 2;
  int values[values_count] = {5, 10};
  VisRing.drawBarChartVert(values, values_count, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_2 = 8;
  int values_2[values_count_2] = {5,10,15,20,30,35,40,45};
  VisRing.drawBarChartVert(values_2, values_count_2, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_3 = 16;
  int values_3[values_count_3] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80};
  VisRing.drawBarChartVert(values_3, values_count_3, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();
}

void loop() {
  example_BarChartVert();
}