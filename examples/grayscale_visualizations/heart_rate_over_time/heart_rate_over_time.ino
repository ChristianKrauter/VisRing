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
  int values_count = 24;

  int hr_standing[values_count] = { 83, 83,  59,  76,  88, 84,  70, 84, 71, 69,  90, 113, 120, 89,  89, 95,  84, 94,  83, 102, 72,  72,  85,  102 };
  int hr_sitting[values_count] =  { 53, 83,  85,  80,  92, 87,  59, 96, 91, 87,  93, 89,   85, 84,  96, 85,  91, 88,  92, 85 , 83,  89,  82,  92  };
  //int hr_walking[values_count] =  { 83, 103, 105, 120, 92, 170, 99, 96, 91, 112, 93, 100, 104, 101, 96, 120, 91, 108, 92, 105, 103, 109, 102, 92 };
  int hr_walking[values_count] =    { 30,30,30,30,30,30,30,30,30,30,30, 30, 200, 101, 96, 120, 91, 108, 92, 105, 103, 109, 102, 92 };

  /*
  VisRing.printStringGS(84, 20, String(hr_standing[values_count / 2]), true, 15, 0);
  VisRing.lineChart(hr_standing, values_count, true, 0, 200, 15);
  VisRing.lineVGS(80, 0, 32, 7);
  VisRing.displayGS();
  delay(2000);
  VisRing.clearDisplayGS(2);

  VisRing.printStringGS(84, 20, String(hr_sitting[values_count / 2]), true, 15, 0);
  VisRing.lineChart(hr_sitting, values_count, true, 0, 200, 15);
  VisRing.lineVGS(80, 0, 32, 7);
  VisRing.displayGS();
  delay(2000);
  VisRing.clearDisplayGS(2);
  */

  VisRing.printStringGS(75, 20, String(hr_walking[values_count / 2]), true, 15, 0);
  VisRing.lineChart(hr_walking, values_count, true, 0, 200, 15);
  VisRing.lineVGS(72, 0, 32, 7);
  VisRing.displayGS();
  delay(2000);
  VisRing.clearDisplayGS(2);

}

void loop() {
  example_HearRateOverTime();
}