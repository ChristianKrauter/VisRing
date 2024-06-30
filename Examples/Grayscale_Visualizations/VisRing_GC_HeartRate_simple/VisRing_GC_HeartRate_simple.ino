/*
  Control a display extended smart ring
  Example: Simple heart rate
*/

#include <SPI.h>
#include <VisRing.h>

// Initialize the display with the follow pin connections
// Note that you should not change SCLK and MOSI because the
// library uses hardware SPI
VisRing VisRing(15, 16, 12, 13);
HardwareSerial *printer = &Serial;

void setup()
{
  SPI.begin();
  Serial.begin(115200);
  VisRing.begin(160, 32, printer); // Display is 160 wide, 32 high
  VisRing.clearDisplayGS();
}

void example_HearRate()
{
  int hr[] = {50, 55, 60, 60, 65, 70, 70, 90, 90, 70, 50, 50};

  for (int val : hr)
  {
    VisRing.drawHR(val, 15, 7);
    VisRing.displayGS();
    delay(500);
    VisRing.clearDisplayGS();
  }
}

void loop()
{
  example_HearRate();
}