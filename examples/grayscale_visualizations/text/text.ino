/*
  Control a display extended smart ring
  Example: Text
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

void example_Text()
{
  String text = "Please remember that we are meeting with Lars today at 4 pm at the bus stop. Can you be there at 3:30?";
  VisRing.printStringGS(0, 24, text, false, 15, 0);
  VisRing.displayGS();
  delay(2000);
  VisRing.clearDisplayGS(2);
}

void loop()
{
  example_Text();
}