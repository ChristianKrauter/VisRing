/*
  Control a display extended smart ring
  Example: Bar Chart for when the ring is at horizontal orientation
*/

#include <SPI.h>
#include <VisRing.h>

// Initialize the display with the follow pin connections
// Note that you should not change SCLK and MOSI because the
// library uses hardware SPI
VisRing VisRing(15, 16, 12, 13); // NRF52832 15 = CS, 16 = RES, 13 = SCLK, 11 = SDIN
HardwareSerial *printer = &Serial;

void setup()
{
  SPI.begin();
  Serial.begin(115200);
  VisRing.begin(160, 32, printer); // Display is 160 wide, 32 high
  VisRing.clearDisplayGS();
}

void example_BarChartHor()
{
  int values_count = 3;
  int values[values_count] = {0, 5, 10};
  VisRing.drawBarChartHor(values, values_count, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_2 = 10;
  int values_2[values_count_2] = {41, 76, 1, 6, 24, 44, 100, 68, 53, 2};
  VisRing.drawBarChartHor(values_2, values_count_2, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_3 = 20;
  int values_3[values_count_3] = {3, 16, 30, 82, 8, 23, 99, 6, 5, 57, 55, 71, 100, 33, 80, 34, 1, 3, 63, 42};
  VisRing.drawBarChartHor(values_3, values_count_3, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_4 = 40;
  int values_4[values_count_4] = {57, 9, 32, 47, 39, 45, 59, 41, 100, 60, 66, 70, 69, 46, 67, 69, 75, 85, 6, 14, 15, 61, 5, 18, 49, 47, 80, 69, 48, 96, 55, 18, 84, 52, 36, 86, 53, 62, 22, 90};
  VisRing.drawBarChartHor(values_4, values_count_4, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_5 = 80;
  int values_5[values_count_5] = {38, 26, 16, 78, 100, 50, 43, 91, 78, 31, 60, 72, 24, 14, 88, 96, 44, 65, 33, 31, 66, 0, 3, 83, 0, 84, 82, 4, 33, 68, 87, 25, 49, 14, 9, 21, 9, 41, 24, 49, 56, 3, 36, 10, 10, 8, 95, 88, 28, 47, 88, 46, 75, 31, 46, 26, 65, 72, 77, 89, 20, 34, 9, 76, 38, 47, 39, 37, 16, 1, 71, 21, 52, 32, 69, 75, 98, 42, 92, 0};
  VisRing.drawBarChartHor(values_5, values_count_5, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();
}

void loop()
{
  example_BarChartHor();
}