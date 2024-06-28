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

void scaleToScreenWidth(int values[], int values_count){
  int max_value = 0;

  for (int i = 0; i < values_count; i++) {
    if (values[i] > max_value) {
     max_value = values[i];
    }
  }

  for (int i = 0; i < values_count; i++) {
    values[i] = (oled.getDisplayWidth() * values[i]) / max_value;
  }
}

void drawBarChartHor(int values[], int values_count, int grayscale){
  if (values_count > 16 | values_count < 2) {
    return;
  }

  scaleToScreenWidth(values, values_count);

  int width = (oled.getDisplayHeight() - (values_count - 1)) / values_count;

  for (int i = 0; i < values_count; i++) {
    oled.rectFillGS(0, i+i*1+i*width, values[i], width, grayscale);
  }
}

void loop() {
  int values_count = 2;
  int values[values_count] = {5, 10};
  drawBarChartHor(values, values_count, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_2 = 8;
  int values_2[values_count_2] = {5,10,15,20,30,35,40,45};
  drawBarChartHor(values_2, values_count_2, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_3 = 16;
  int values_3[values_count_3] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80};
  drawBarChartHor(values_3, values_count_3, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();
}