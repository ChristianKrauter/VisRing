/*
  Control a display extended smart ring
  Example: Notifications
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

void example_Notifications()
{
  int len = 9;
  int notification_counts[] = {1, 2, 3, 4, 4, 5, 5, 5, 6};

  for (int i = 0; i < len; i++)
  {
    VisRing.showNotification(notification_counts[i], 10);
    VisRing.displayGS();
    delay(1000);
    VisRing.clearDisplayGS(0);
  }
}

void loop()
{
  example_Notifications();
}