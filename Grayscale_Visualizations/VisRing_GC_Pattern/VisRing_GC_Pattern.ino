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

  drawPattern(1);
  oled.displayGS();
}

void drawPattern(int type) {
  switch (type) {
  case 0:
    for (int i = 0; i <= oled.getDisplayWidth() / 10; i ++) {
      oled.lineGS(i*10, 0, (i+1)*10, 30, i % 14 + 1);
      oled.lineGS((i+1)*10, 0, i*10, 30, (16-i) % 14 + 1);
    }
  case 1:
    for (int i = 0; i < oled.getDisplayHeight()/2; i ++) {
      oled.lineGS(0, i, 160, i, i);
      oled.lineGS(0, i+15, 160, i+15, (16-i));
    }
  }
}

void loop() {

}