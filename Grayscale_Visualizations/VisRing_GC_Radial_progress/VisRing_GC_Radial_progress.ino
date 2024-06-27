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

void setup() {
  SPI.begin();
  Serial.begin(115200);
  oled.begin(160, 32, printer);  //Display is 160 wide, 32 high
  oled.clearDisplayGS();
}

void drawCircleSegmentGS(int x0, int y0, int x, int y, int parts, int grayscale)
{
  if (parts >= 1){
    oled.setPixelGS(x0+x, y0+y, grayscale);
  }

  if (parts >= 2){
    oled.setPixelGS(x0+y, y0+x, grayscale);
  }

  if (parts >= 3){
    oled.setPixelGS(x0+y, y0-x, grayscale);
  }

  if (parts >= 4){
    oled.setPixelGS(x0+x, y0-y, grayscale);
  }

  if (parts >= 5){
    oled.setPixelGS(x0-x, y0-y, grayscale);
  }

  if (parts >= 6){
    oled.setPixelGS(x0-y, y0-x, grayscale);
  }

  if (parts >= 7){
    oled.setPixelGS(x0-y, y0+x, grayscale);
  }

  if (parts >= 8){
    oled.setPixelGS(x0-x, y0+y, grayscale);    
  }
}

/** \brief Draw grayscale circle with Bresenham’s algorithm.
  Draw circle with radius using 4bit grayscale color (0-15) at x,y of the screen buffer.
*/
void radialProgress(int x0, int y0, int radius, int parts, int grayscale_background, int grayscale) {
  int d = 3 - 2 * radius;
  int x = 0;
  int y = radius;

  oled.drawCircleGS(x0, y0, x, y, grayscale_background);
  drawCircleSegmentGS(x0, y0, x, y, parts, grayscale);

  while (x <= y) {
    x++;

    if (d > 0) {
      y--;
      d = d + 4 * (x-y) + 10;
    }
    else {
      d = d + 4 * x + 6;
    }
    
    oled.drawCircleGS(x0, y0, x, y, grayscale_background);
    drawCircleSegmentGS(x0, y0, x, y, parts, grayscale);
  }
}

void loop() {
  for (int i = 0; i <= 8; i++) {
    oled.clearDisplayGS();
    radialProgress(80, 15, 15, i, 1, 15);
    int x = 75;
    if (i == 8) {
      x = 70;
    }
    if (i == 0) {
      x = 78;
    }
    oled.printStringGS(x,12,String(int(i*12.5)),15,0);
    oled.displayGS();
    delay(1000);
    if (i == 9) {
      i == 0;
    }
  }

}