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

// Center and print a small text
// This function is quick and dirty. Only works for texts one line long.
void printText(String text, int font)
{
  int middleX = oled.getDisplayWidth() / 2;
  int middleY = oled.getDisplayHeight() / 2;

  oled.clearDisplay();
  oled.setFontType(font);

  // Set the cursor in the middle of the screen
  oled.setCursor(middleX - (oled.getFontWidth() * (text.length() / 2)),
                         middleY - (oled.getFontHeight() / 2));

  // Print the text:
  oled.print(text);
  oled.display();

  delay(1500);

  oled.clearDisplay(); //Clear everything
}

void showNotification(int number){
  String text = String(number);
   
  if(number==1){
     text += " new message";
  }
  else {
    text += " new messages";
  }

  oled.clearDisplayGS(0);
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
}

void loop() {
  int number = 0;

  number = 1;
  showNotification(number);
  delay(3000);

  number = 2;
  showNotification(number);
  delay(3000);

  number = 3;
  showNotification(number);
  delay(3000);

  number = 4;
  showNotification(number);
  delay(3000);

  number = 5;
  showNotification(number);
  delay(3000);

  number = 0;
  showNotification(number);
  delay(6000);
}