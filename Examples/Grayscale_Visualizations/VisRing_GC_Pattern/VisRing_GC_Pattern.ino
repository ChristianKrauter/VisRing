/*
  Control a display extended smart ring
  Example: Pattern
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

void example_Pattern()
{
  for (int i = 0; i < 3; i++)
  {
    VisRing.drawPattern(i);
    VisRing.displayGS();
    delay(500);
    VisRing.clearDisplayGS();
  }
}

void loop()
{
  example_Pattern();
}