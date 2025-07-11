/*
  Control a display extended smart ring
  Example: Line Chart
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
  VisRing.clearDisplayGS(2);
}

void example_lineChart()
{
  int values_count = 2;
  int values[values_count] = {0, 10};
  VisRing.lineChart(values, values_count, false, 0, 0, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS(2);

  int values_count_2 = 8;
  int values_2[values_count_2] = {80, 12, 55, 19, 74, 54, 58, 82};
  VisRing.lineChart(values_2, values_count_2, false, 0, 0, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS(2);

  int values_count_3 = 16;
  int values_3[values_count_3] = {14, 38, 38, 27, 20, 91, 11, 38, 41, 56, 23, 77, 0, 0, 47, 56};
  VisRing.lineChart(values_3, values_count_3, false, 0, 0, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS(2);

  int values_count_4 = 40;
  int values_4[values_count_4] = {29, 51, 34, 44, 90, 44, 12, 16, 75, 0, 47, 57, 70, 1, 45, 49, 36, 74, 73, 30, 70, 8, 71, 22, 11, 37, 49, 83, 37, 40, 30, 36, 25, 50, 48, 7, 91, 64, 26, 64};
  VisRing.lineChart(values_4, values_count_4, false, 0, 0, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS(2);

  int values_count_5 = 80;
  int values_5[values_count_5] = {1, 47, 6, 41, 18, 9, 50, 56, 80, 25, 63, 29, 42, 15, 98, 78, 36, 99, 64, 12, 55, 49, 7, 14, 85, 68, 24, 17, 74, 77, 40, 27, 4, 89, 39, 19, 13, 94, 83, 91, 53, 20, 60, 66, 87, 93, 2, 31, 3, 54, 84, 45, 65, 46, 16, 86, 21, 59, 28, 10, 92, 70, 37, 38, 34, 75, 81, 67, 52, 8, 62, 73, 58, 26, 61, 43, 0, 35, 82, 23};
  VisRing.lineChart(values_5, values_count_5, false, 0, 0, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS(2);

  int values_count_6 = 160;
  int values_6[values_count_6] = {89, 58, 98, 73, 79, 80, 10, 7, 25, 45, 27, 59, 34, 94, 23, 5, 46, 95, 84, 75, 33, 63, 57, 28, 75, 93, 71, 52, 93, 31, 92, 33, 65, 6, 67, 53, 35, 64, 66, 30, 25, 92, 53, 69, 60, 68, 90, 63, 2, 37, 72, 75, 27, 68, 91, 17, 29, 16, 78, 94, 51, 32, 73, 8, 28, 16, 61, 19, 63, 17, 34, 71, 64, 39, 70, 55, 48, 53, 49, 42, 57, 22, 63, 4, 48, 30, 18, 16, 37, 3, 88, 80, 99, 52, 71, 48, 27, 0, 94, 13, 75, 33, 33, 96, 65, 36, 16, 20, 52, 50, 54, 95, 55, 68, 0, 74, 79, 48, 18, 38, 5, 41, 7, 35, 11, 26, 20, 18, 5, 9, 65, 96, 34, 27, 21, 28, 61, 83, 6, 95, 41, 89, 7, 69, 59, 61, 92, 4, 44, 99, 12, 47, 15, 84, 29, 18, 75, 45, 83, 84};
  VisRing.lineChart(values_6, values_count_6, false, 0, 0, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS(2);
}

void loop()
{
  example_lineChart();
}