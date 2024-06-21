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

void drawBackground(){
  for (int i = 15; i < 65; i++) {
    oled.lineGS(i, 0, i+10, oled.getDisplayHeight(), 4);
  }
  for (int i = 70; i < 100; i+=8) {
    oled.lineGS(i, 10, i+4, 14, 15);
    oled.lineGS(i+4, 14, i+8, 10, 15);
  }
  oled.lineGS(102, 10, 106, 14, 15);

  int startX = 104;
  int lowestY = 10;

  oled.lineGS(startX, lowestY+10, startX, lowestY+6, 15);
  oled.lineGS(startX+1, lowestY+11, startX+1, lowestY+5, 15);
  oled.lineGS(startX+2, lowestY+12, startX+2, lowestY+4, 15);
  oled.lineGS(startX+3, lowestY+12, startX+3, lowestY+3, 15);
  oled.lineGS(startX+4, lowestY+12, startX+4, lowestY+2, 15);
  oled.lineGS(startX+5, lowestY+11, startX+5, lowestY+1, 15);
  
  oled.lineGS(startX+6, 20, startX+6, lowestY, 15);
  
  oled.lineGS(startX+7, lowestY+11, startX+7, lowestY+1, 15);
  oled.lineGS(startX+8, lowestY+12, startX+8, lowestY+2, 15);
  oled.lineGS(startX+9, lowestY+12, startX+9, lowestY+3, 15);
  oled.lineGS(startX+10, lowestY+12, startX+10, lowestY+4, 15);
  oled.lineGS(startX+11, lowestY+11, startX+11, lowestY+5, 15);
  oled.lineGS(startX+12, lowestY+10, startX+12, lowestY+6, 15);

}


void loop() {
  String text = "50 bpm";

  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "55 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "60 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "70 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "70 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "70 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "70 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "50 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "50 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "40 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

  text = "40 bpm";
  oled.clearDisplayGS(0);
  drawBackground();
  oled.printStringGS(25,10,text,15,0);
  oled.displayGS();
  delay(500);

}