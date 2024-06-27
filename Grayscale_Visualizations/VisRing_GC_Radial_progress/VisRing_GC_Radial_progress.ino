/*
  Control a SSD1320 based flexible OLED display
  By: Nathan Seidle
  SparkFun Electronics
  Date: November 21st, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  A silly pong example. Comes from Marshall's tutorial:
  https://learn.sparkfun.com/tutorials/teensyview-hookup-guide#example-prop-shield-compatible-connection

  On an 8MHz Arduino Pro Mini this is pretty painfully slow.

  To connect the display to an Arduino:
  (Arduino pin) = (Display pin)
  Pin 13 = SCLK on display carrier
  11 = SDIN
  10 = !CS
  9 = !RES

  The display is 160 pixels long and 32 pixels wide
  Each 4-bit nibble is the 4-bit grayscale for that pixel
  Therefore each byte of data written to the display paints two sequential pixels
  Loops that write to the display should be 80 iterations wide and 32 iterations tall
*/

#include <SPI.h>
#include <SSD1320_OLED.h>
#include <string>

//Initialize the display with the follow pin connections
//Note that you should not change SCLK and MOSI because the
//library uses hardware SPI
SSD1320 oled(15, 16, 12, 13);  // NRF52832 15 = CS, 16 = RES, 13 = SCLK, 11 = SDIN
HardwareSerial* printer = &Serial;

int circle_lookup[30] = {
  1,
  4,
  8,
  8,
  12,
  16,
  16,
  20,
  24,
  29,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1
};

void setup() {
  SPI.begin();
  Serial.begin(115200);
  oled.begin(160, 32, printer);  //Display is 160 wide, 32 high
  oled.clearDisplayGS();
}

void drawCircleGS(int x0, int y0, int x, int y, int grayscale)
{
    oled.setPixelGS(x0+x, y0+y, grayscale);
    oled.setPixelGS(x0-x, y0+y, grayscale);
    oled.setPixelGS(x0+x, y0-y, grayscale);
    oled.setPixelGS(x0-x, y0-y, grayscale);
    oled.setPixelGS(x0+y, y0+x, grayscale);
    oled.setPixelGS(x0-y, y0+x, grayscale);
    oled.setPixelGS(x0+y, y0-x, grayscale);
    oled.setPixelGS(x0-y, y0-x, grayscale);
}

/** \brief Draw grayscale circle with Bresenham’s algorithm.
  Draw circle with radius using 4bit grayscale color (0-15) at x,y of the screen buffer.
*/
void radialProgress(int x0, int y0, int radius, int percent, int grayscale) {
  int d = 3 - 2 * radius;
  int x = 0;
  int y = radius;

  while (x <= circle_lookup[radius]*percent) {
    x++;

    if (d > 0) {
      y--;
      d = d + 4 * (x-y) + 10;
    }
    else {
      d = d + 4 * x + 6;
    }

    oled.setPixelGS(x0+x, y0+y, grayscale);
  }
}

void drawArc(int x0, int y0, int radius, int percent, int grayscale){

}
int start_angle = 0
int end_angle = 1.1, r = 30;
for(var i = start_angle; i < end_angle; i = i + 0.05)
{
  drawpixel(x: 50 + Math.cos(i) * r, y: 100 + Math.sin(i) * r); // center point is (x = 50, y = 100)
}

void loop() {
  radialProgress(80,15, 30, 1, 15);
  oled.displayGS();
}