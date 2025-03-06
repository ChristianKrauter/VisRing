/*
  MAX30105 Breakout: Output all the raw Red/IR/Green readings
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 2nd, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  Outputs all Red/IR/Green values.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).
*/

// General
#include <Wire.h>
#include <InternalFileSystem.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

// temp
#include <Adafruit_TMP117.h>

// oled
#include <SSD1320_OLED.h>

//////////////////////
/////// general //////
//////////////////////

long display_time = millis();

int total_num_data = 14;

float ALL_MAX[14] = {
  -1, -1, -1,
  -1, -1, -1,
  -1, -1, -1,
  -1, -1, -1,
  -1, -1
};

//////////////////////
//////// oled ////////
//////////////////////

SSD1320 flexibleOLED(15, 16, 12, 13);  // NRF52832 15 = CS, 16 = RES, 13 = SCLK, 11 = SDIN

void oled_setup() {
  flexibleOLED.begin(160, 32);  // Display is 160 wide, 32 high
  flexibleOLED.clearDisplay();
}

//////////////////////
///// tmp sensor /////
//////////////////////
Adafruit_TMP117 tmp117;

void tmp_setup() {
  // Try to initialize!
  if (!tmp117.begin()) {
    Serial.println("Failed to find TMP117 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("TMP117 Success!");
  tmp117.setAveragedSampleCount(TMP117_AVERAGE_1X);
  tmp117.setReadDelay(TMP117_DELAY_0_MS);
}

int get_tmp_data() {
  sensors_event_t temp;  // create an empty event to be filled
  tmp117.getEvent(&temp);
  return temp.temperature;
}

void display_tmp(int value) {
  flexibleOLED.setFontType(0);  // Large font
  flexibleOLED.setCursor(25, 12);
  flexibleOLED.print("Temp: ");
  flexibleOLED.print(value);
  flexibleOLED.display();
  Serial.println(value);
}

//////////////////
////// main //////
//////////////////

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  oled_setup();
  tmp_setup();
}

void clear_all_max() {
  for (int i = 0; i < total_num_data; i++) {
    ALL_MAX[i] = -1;
  }
}

void print_all_max() {
  Serial.print("[");
  for (int i = 0; i < total_num_data; i++) {
    Serial.print(ALL_MAX[i]);
    Serial.print(",");
  }
  Serial.println("]");
}

void loop() {
  display_tmp(get_tmp_data());
}
