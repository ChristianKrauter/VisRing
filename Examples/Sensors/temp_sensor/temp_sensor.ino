/*
  VisRing code to read and display temperature values.
*/

// General
#include <Wire.h>
#include <InternalFileSystem.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

// temp
#include <Adafruit_TMP117.h>

// oled
#include <VisRing.h>

VisRing VisRing(15, 16, 12, 13);

float data = -1;

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

void get_tmp_data() {
  data = -1;
  sensors_event_t temp;  // create an empty event to be filled
  tmp117.getEvent(&temp);
  data = temp.temperature;
}

void display_tmp() {
  if (data != -1) {
    VisRing.clearDisplayGS();

    VisRing.printStringGS(
      10,
      12,
      "temp:" + (String)data,
      false,
      15,
      0);

    VisRing.displayGS();
    print_data();
  }
}

void print_data() {
  Serial.println("temp: " + (String)data);
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  tmp_setup();

  VisRing.begin(160, 32);  // Display is 160 wide, 32 high
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
}

void loop() {
  get_tmp_data();
  display_tmp();
  delay(1000);
}
