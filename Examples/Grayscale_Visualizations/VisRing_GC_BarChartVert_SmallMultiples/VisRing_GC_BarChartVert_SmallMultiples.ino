/*
  Control a display extended smart ring
  Example: Small multiples Bar Chart for when the ring is at vertical orientation
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

void example_BarChartVert_SmallMultiples()
{
  int values_0[7][16] = {{55, 88, 2, 5, 23, 49, 83}, {39, 95, 64, 56, 11, 55, 75}};
  int charts_count_0 = 2;
  int values_counts_0[7] = {7, 7};
  bool focus_charts_0[7] = {true, false, false, false, false, false, false};
  int value_range_min = 0;
  int value_range_max_0 = 100;

  VisRing.drawBarChartVertSmallMultiples(values_0, charts_count_0, values_counts_0, focus_charts_0, value_range_min, value_range_max_0, 4, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_1[7][16] = {{64, 44, 20, 67, 28, 11, 66, 26, 70, 76, 30, 22, 21, 37, 82, 36}, {72, 69, 52, 46, 29, 59, 73, 25, 48, 87, 89, 5, 43, 35, 60, 38}, {97, 24, 74, 50, 39, 88, 2, 9, 4, 15, 71, 34, 96, 27, 91, 40}, {12, 100, 99, 77, 18, 83, 19, 54, 78, 80, 92, 62, 42, 85, 93, 68}};
  int charts_count_1 = 4;
  int values_counts_1[7] = {16, 16, 16, 16, 0, 0, 0};
  bool focus_charts_1[7] = {true, true, true, true, false, false, false};
  int value_range_min_1 = 0;
  int value_range_max_1 = 100;

  VisRing.drawBarChartVertSmallMultiples(values_1, charts_count_1, values_counts_1, focus_charts_1, value_range_min_1, value_range_max_1, 4, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_2[7][16] = {{21, 17, 72, 81, 50}, {47, 65, 98, 51, 63}, {69, 73, 31, 92, 39}, {90, 24, 14, 10, 53}, {68, 94, 55, 100, 77}, {82, 25, 83, 27, 45}, {4, 44, 99, 59, 23}};
  int charts_count_2 = 7;
  int values_counts_2[7] = {5, 5, 5, 5, 5, 5, 5};
  bool focus_charts_2[7] = {false, false, false, true, false, false, false};
  int value_range_min_2 = 0;
  int value_range_max_2 = 100;

  VisRing.drawBarChartVertSmallMultiples(values_2, charts_count_2, values_counts_2, focus_charts_2, value_range_min_2, value_range_max_2, 4, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();
}

void loop()
{
  example_BarChartVert_SmallMultiples();
}