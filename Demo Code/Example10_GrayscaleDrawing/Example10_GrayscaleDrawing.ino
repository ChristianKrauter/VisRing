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
SSD1320 flexibleOLED(15, 16, 12, 13);  // NRF52832 15 = CS, 16 = RES, 13 = SCLK, 11 = SDIN
HardwareSerial* printer = &Serial;

void setup() {
  SPI.begin();
  Serial.begin(115200);
  flexibleOLED.begin(160, 32, printer);  //Display is 160 wide, 32 high

  flexibleOLED.displayGS();
  delay(2000);
  flexibleOLED.clearDisplayGS();
}

void testLineGS()
{
  for (int x = 0; x < flexibleOLED.getDisplayWidth(); x ++) {
    flexibleOLED.lineGS(x, 0, x, flexibleOLED.getDisplayHeight(), x % 16);
  }
}

void testRectGS()
{
  flexibleOLED.rectGS(0, 0, 15, flexibleOLED.getDisplayHeight(), 5);
  flexibleOLED.rectGS(30, 0, 45, flexibleOLED.getDisplayHeight()/2, 10);
  flexibleOLED.rectFillGS(100, 15, 100, (flexibleOLED.getDisplayHeight()/3)*2, 15);
}

void testCircleGS()
{
  flexibleOLED.circleGS(0, 0, 20, 5);
  flexibleOLED.circleFillGS(50, 8, 20, 10);
  flexibleOLED.circleGS(130, 16, 12, 15);
  flexibleOLED.circleFillGS(130, 16, 5, 15);
}

// Center and print a small text
// This function is quick and dirty. Only works for texts one line long.
void printText(String text, int font)
{
  int middleX = flexibleOLED.getDisplayWidth() / 2;
  int middleY = flexibleOLED.getDisplayHeight() / 2;

  flexibleOLED.clearDisplay();
  flexibleOLED.setFontType(font);

  // Set the cursor in the middle of the screen
  flexibleOLED.setCursor(middleX - (flexibleOLED.getFontWidth() * (text.length() / 2)),
                         middleY - (flexibleOLED.getFontHeight() / 2));

  // Print the text:
  flexibleOLED.print(text);
  flexibleOLED.display();

  delay(1500);

  flexibleOLED.clearDisplay(); //Clear everything
}

void testsetPixelGS(){
  for (int rows = 0 ; rows < 32 ; rows++){
    for (int columns = 0 ; columns < 160; columns++){
      flexibleOLED.setPixelGS(columns, rows, 4);
    }
  }
}

void loop() {
  printText("Test 1: All pixels", 0);
  delay(2000);
  flexibleOLED.clearDisplayGS();
  
  testsetPixelGS();
  flexibleOLED.displayGS();
  delay(2000);
  flexibleOLED.clearDisplayGS();

  printText("Test 2: Lines", 0);
  delay(2000);
  flexibleOLED.clearDisplayGS();
  testLineGS();
  flexibleOLED.displayGS();
  delay(2000);
  flexibleOLED.clearDisplayGS(0);

  printText("Test 3: Rectangles", 0);
  delay(2000);
  flexibleOLED.clearDisplayGS();
  testRectGS();
  flexibleOLED.displayGS();
  delay(2000);
  flexibleOLED.clearDisplayGS(0);

  printText("Test 4: Circles", 0);
  delay(2000);
  flexibleOLED.clearDisplayGS();
  testCircleGS();
  flexibleOLED.displayGS();
  delay(2000);
  flexibleOLED.clearDisplayGS();
}