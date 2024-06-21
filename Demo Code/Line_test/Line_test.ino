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


//Initialize the display with the follow pin connections
//Note that you should not change SCLK and MOSI because the
//library uses hardware SPI
SSD1320 flexibleOLED(15, 16, 12, 13); // NRF52832 15 = CS, 16 = RES, 13 = SCLK, 11 = SDIN

void setup()
{
  SPI.begin();
  //  SSD1320 flexibleOLED(10, 9); //10 = CS, 9 = RES, 13 = SCLK, 11 = SDIN
  Serial.begin(115200);
  HardwareSerial* printer;
  printer = &Serial;
  flexibleOLED.begin(160, 32, printer); //Display is 160 wide, 32 high
  flexibleOLED.clearDisplayGS();

  flexibleOLED.displayGS();

  uint8_t SIZE = 20;
  uint8_t hr[SIZE] = {3,2,1,5,10,15,20,25,30,30,20,10,8,7,6,5,4,3,2,1}; // Heart Rate Values over fixed time steps
  uint8_t timeStepSize = 5;
  uint8_t grayscale = 3;

  lineChart(0,0,hr,SIZE, timeStepSize, grayscale);
}

void loop()
{

}

void lineChart(uint8_t x_0, uint8_t y_0, uint8_t hr[], uint8_t SIZE, uint8_t timeStepSize, uint8_t grayscale){
  for (int i = 0; i < SIZE-1; i++) {
    flexibleOLED.lineGS(x_0 + i*timeStepSize, hr[i] + y_0, x_0 + (i+1)*timeStepSize, hr[i+1] + y_0, grayscale);
  }
  flexibleOLED.displayGS();
}