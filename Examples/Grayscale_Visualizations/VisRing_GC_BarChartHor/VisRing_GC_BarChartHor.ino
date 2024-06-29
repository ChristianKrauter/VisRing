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

void scaleToScreenHeight(int values[], int values_count){
  int max_value = 0;
  int scaled_values[] = {};

  for (int i = 0; i < values_count; i++) {
    if (values[i] > max_value) {
     max_value = values[i];
    }
  }

  for (int i = 0; i < values_count; i++) {
    values[i] = (oled.getDisplayHeight() * values[i]) / max_value;
  }
}

void drawBarChartVert(int values[], int values_count, int grayscale){
  if (values_count > 80 | values_count < 2) {
    return;
  }

  scaleToScreenHeight(values, values_count);

  int width = (oled.getDisplayWidth() - (values_count - 1)) / values_count;

  for (int i = 0; i < values_count; i++) {
    oled.rectFillGS(i+i*1+i*width, 0, width, values[i], grayscale);
  }
}

void loop() {
  int values_count = 2;
  int values[values_count] = {5, 10};
  drawBarChartVert(values, values_count, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_2 = 10;
  int values_2[values_count_2] = {41, 76, 1, 6, 24, 44, 0, 68, 53, 2};
  drawBarChartVert(values_2, values_count_2, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_3 = 20;
  int values_3[values_count_3] = {3, 16, 30, 82, 8, 23, 99, 6, 5, 57, 55, 71, 85, 33, 80, 34, 1, 3, 63, 42};
  drawBarChartVert(values_3, values_count_3, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_4 = 40;
  int values_4[values_count_4] = {57, 9, 32, 47, 39, 45, 59, 41, 67, 60, 66, 70, 69, 46, 67, 69, 75, 85, 6, 14, 15, 61, 5, 18, 49, 47, 80, 69, 48, 96, 55, 18, 84, 52, 36, 86, 53, 62, 22, 90};
  drawBarChartVert(values_4, values_count_4, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_5 = 80;
  int values_5[values_count_5] = {38, 26, 16, 78, 2, 50, 43, 91, 78, 31, 60, 72, 24, 14, 88, 96, 44, 65, 33, 31, 66, 0, 3, 83, 0, 84, 82, 4, 33, 68, 87, 25, 49, 14, 9, 21, 9, 41, 24, 49, 56, 3, 36, 10, 10, 8, 95, 88, 28, 47, 88, 46, 75, 31, 46, 26, 65, 72, 77, 89, 20, 34, 9, 76, 38, 47, 39, 37, 16, 1, 71, 21, 52, 32, 69, 75, 98, 42, 92, 0};
  drawBarChartVert(values_5, values_count_5, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();
}