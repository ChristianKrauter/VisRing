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
  VisRing.clearDisplayGS(2);
}

void example_HearRateOverTime() {
  int values_count = 80;
  // Copilot numbers 60 - 100
  // int hr[values_count] = { 72, 85, 91, 78, 94, 67, 82, 76, 88, 73, 97, 69, 81, 79, 92, 70, 83, 75, 89, 68, 84, 77, 93, 71, 86, 80, 95, 66, 87, 74, 90, 65, 96, 63, 98, 64, 99, 62, 100, 61, 97, 63, 95, 66, 92, 68, 89, 70, 87, 71, 85, 72, 83, 73, 81, 74, 79, 75, 77, 76, 78, 80, 76, 82, 75, 84, 74, 86, 73, 88, 72, 90, 71, 91, 70, 92, 69, 93, 68, 94 };
  // Copilot numbers 0 - 180
  // int hr[values_count] = { 112, 129, 147, 92, 163, 75, 138, 120, 156, 81, 174, 69, 141, 102, 167, 60, 132, 84, 150, 66, 144, 96, 159, 72, 135, 78, 153, 63, 126, 90, 165, 57, 177, 54, 180, 51, 179, 48, 178, 45, 176, 42, 174, 39, 172, 36, 170, 33, 168, 30, 166, 27, 164, 24, 162, 21, 160, 18, 158, 15, 156, 12, 154, 9, 152, 6, 150, 3, 148, 0, 146, 3, 144, 6, 142, 9, 140, 12, 138, 15 };
  // RNG
  int hr[values_count] = { 107, 151, 51, 162, 85, 164, 57, 130, 38, 110, 163, 181, 177, 178, 71, 84, 75, 23, 97, 158, 163, 78, 121, 160, 11, 100, 74, 116, 120, 181, 53, 63, 98, 78, 8, 114, 115, 5, 135, 14, 180, 90, 84, 189, 186, 141, 13, 133, 113, 71, 182, 153, 177, 62, 78, 98, 125, 65, 42, 96, 80, 31, 174, 92, 138, 88, 70, 159, 185, 133, 26, 40, 84, 92, 141, 175, 63, 162, 137, 58 };
  VisRing.lineChart(hr, values_count, true, 0, 200, 15);
  VisRing.printStringGS(80, 15, String(hr[40]), true, 0, 0);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS(2);
}

void loop() {
  example_HearRateOverTime();
}