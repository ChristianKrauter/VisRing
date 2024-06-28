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

void lineChart(int values[], int values_count, int grayscale){
  if (values_count > 160 | values_count < 2) {
    return;
  }

  scaleToScreenHeight(values, values_count);
  int step_size = oled.getDisplayWidth()/(values_count-1);

  for (int i = 0; i < values_count-1; i++) {
    oled.lineGS(i*step_size, values[i], (i+1)*step_size, values[i+1], grayscale);
  }
}

void loop() {
  int values_count = 2;
  int values[values_count] = {0, 10};
  lineChart(values, values_count, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_2 = 8;
  int values_2[values_count_2] = {80, 12, 55, 19, 74, 54, 58, 82};
  lineChart(values_2, values_count_2, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_3 = 16;
  int values_3[values_count_3] = {14, 38, 38, 27, 20, 91, 11, 38, 41, 56, 23, 77, 0, 0, 47, 56};
  lineChart(values_3, values_count_3, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_4 = 40;
  int values_4[values_count_4] = {29, 51, 34, 44, 90, 44, 12, 16, 75, 0, 47, 57, 70, 1, 45, 49, 36, 74, 73, 30, 70, 8, 71, 22, 11, 37, 49, 83, 37, 40, 30, 36, 25, 50, 48, 7, 91, 64, 26, 64};
  lineChart(values_4, values_count_4, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_5 = 80;
  int values_5[values_count_5] = {1, 47, 6, 41, 18, 9, 50, 56, 80, 25, 63, 29, 42, 15, 98, 78, 36, 99, 64, 12, 55, 49, 7, 14, 85, 68, 24, 17, 74, 77, 40, 27, 4, 89, 39, 19, 13, 94, 83, 91, 53, 20, 60, 66, 87, 93, 2, 31, 3, 54, 84, 45, 65, 46, 16, 86, 21, 59, 28, 10, 92, 70, 37, 38, 34, 75, 81, 67, 52, 8, 62, 73, 58, 26, 61, 43, 0, 35, 82, 23};
  lineChart(values_5, values_count_5, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();

  int values_count_6 = 160;
  int values_6[values_count_6] = {89, 58, 98, 73, 79, 80, 10, 7, 25, 45, 27, 59, 34, 94, 23, 5, 46, 95, 84, 75, 33, 63, 57, 28, 75, 93, 71, 52, 93, 31, 92, 33, 65, 6, 67, 53, 35, 64, 66, 30, 25, 92, 53, 69, 60, 68, 90, 63, 2, 37, 72, 75, 27, 68, 91, 17, 29, 16, 78, 94, 51, 32, 73, 8, 28, 16, 61, 19, 63, 17, 34, 71, 64, 39, 70, 55, 48, 53, 49, 42, 57, 22, 63, 4, 48, 30, 18, 16, 37, 3, 88, 80, 99, 52, 71, 48, 27, 0, 94, 13, 75, 33, 33, 96, 65, 36, 16, 20, 52, 50, 54, 95, 55, 68, 0, 74, 79, 48, 18, 38, 5, 41, 7, 35, 11, 26, 20, 18, 5, 9, 65, 96, 34, 27, 21, 28, 61, 83, 6, 95, 41, 89, 7, 69, 59, 61, 92, 4, 44, 99, 12, 47, 15, 84, 29, 18, 75, 45, 83, 84};
  lineChart(values_6, values_count_6, 15);
  oled.displayGS();
  delay(500);
  oled.clearDisplayGS();
}