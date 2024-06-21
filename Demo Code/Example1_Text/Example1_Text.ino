/*
  Control a SSD1320 based flexible OLED display
  By: Nathan Seidle
  SparkFun Electronics
  Date: November 21st, 2017
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Basic 'hello world' example.

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

// inslude the SPI library:
#include <SPI.h>
const int slaveSelectPin = 10;

#include <SSD1320_OLED.h>

//Initialize the display with the follow pin connections
//SSD1320 flexibleOLED(9, 10, 13, 11); //10 = CS, 9 = RES, 13 = SCLK, 11 = SDIN

//SSD1320 flexibleOLED(10, 9, 26, 25);

void setup()
{
  //  pinMode (slaveSelectPin, OUTPUT);
  SPI.begin();

  SSD1320 flexibleOLED(15, 16, 12, 13); // NRF52832 15 = CS, 16 = RES, 13 = SCLK, 11 = SDIN
  Serial.begin(115200);

  flexibleOLED.begin(160, 32); //Display is 160 wide, 32 high

  flexibleOLED.clearDisplay(); //Clear display and buffer

  flexibleOLED.setFontType(0); //Large font
  flexibleOLED.setCursor(0, 12);
  flexibleOLED.print("hi Mahanth Gowda");

  flexibleOLED.setFontType(0); //Small font
  flexibleOLED.setCursor(52, 0);
  flexibleOLED.print("8:45:03 AM");

  flexibleOLED.display();
}

void loop()
{

}
