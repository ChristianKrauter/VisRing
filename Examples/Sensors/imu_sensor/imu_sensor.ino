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
#include <Adafruit_Sensor.h>
#include <SPI.h>

// imu
#include "ICM_20948.h"  // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
#define CS_PIN 2        // Which pin you connect CS to. Used only when "USE_SPI" is defined

// oled
#include <SSD1320_OLED.h>

//////////////////////
/////// general //////
//////////////////////

long display_time = millis();

float data[9] = {
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
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
///// imu sensor /////
//////////////////////

ICM_20948_SPI imu;  // If using SPI create an ICM_20948_SPI object

void imu_setup() {
  imu.enableDebugging();  // Uncomment this line to enable helpful debug messages on Serial

  bool initialized = false;
  while (!initialized) {

    imu.begin(CS_PIN, SPI);

    imu.startupDefault(false);  // Force a full - not minimal - startup
    Serial.print(F("Initialization of the sensor returned: "));
    Serial.println(imu.statusString());
    if (imu.status != ICM_20948_Stat_Ok) {
      Serial.println("Trying again...");
      delay(500);
    } else {
      initialized = true;
    }
  }
  Serial.println("ICM20948 Success!");
}

void get_imu_data(void) {
  clear_data();

  if (imu.dataReady()) {
    imu.getAGMT();                 // The values are only updated when you call 'getAGMT'
    data[0] = imu.accX() * 0.01f;  // acceleration is measured in m/s^2
    data[1] = imu.accY() * 0.01f;
    data[2] = imu.accZ() * 0.01f;
    data[3] = imu.gyrX();  // gyro is measured in radians/s
    data[4] = imu.gyrY();
    data[5] = imu.gyrZ();
    data[6] = imu.magX();  // magnetometer is measured in uT
    data[7] = imu.magY();
    data[8] = imu.magZ();
  } else {
    delay(500);
  }
}

void display_imu() {
  if (data[0] != -1) {

    flexibleOLED.setFontType(0);
    flexibleOLED.setCursor(10, 0);
    flexibleOLED.print("acc X: ");
    flexibleOLED.print(data[0], 0);
    flexibleOLED.print("  Y: ");
    flexibleOLED.print(data[1], 0);
    flexibleOLED.print("  Z: ");
    flexibleOLED.println(data[2], 0);

    flexibleOLED.setCursor(10, 12);
    flexibleOLED.print("gyr X: ");
    flexibleOLED.print(data[3], 0);
    flexibleOLED.print("  Y: ");
    flexibleOLED.print(data[4], 0);
    flexibleOLED.print("  Z: ");
    flexibleOLED.print(data[5], 0);

    flexibleOLED.setCursor(10, 24);
    flexibleOLED.print("mag X: ");
    flexibleOLED.print(data[6], 0);
    flexibleOLED.print("  Y: ");
    flexibleOLED.print(data[7], 0);
    flexibleOLED.print("  Z: ");
    flexibleOLED.print(data[8], 0);
    flexibleOLED.display();

    print_data();
  }
}

//////////////////
////// main //////
//////////////////

void clear_data() {
  for (int i = 0; i < 9; i++) {
    data[i] = -1;
  }
}

void print_data()
{
  Serial.print("acc X: ");
  Serial.print(data[0]);
  Serial.print("  Y: ");
  Serial.print(data[1]);
  Serial.print("  Z: ");
  Serial.print(data[2]);

  Serial.print(" - ");
  
  Serial.print("gyr X: ");
  Serial.print(data[3]);
  Serial.print("  Y: ");
  Serial.print(data[4]);
  Serial.print("  Z: ");
  Serial.print(data[5]);

  Serial.print(" - ");

  Serial.print("mag X: ");
  Serial.print(data[6]);
  Serial.print("  Y: ");
  Serial.print(data[7]);
  Serial.print("  Z: ");
  Serial.println(data[8]);
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  oled_setup();
  imu_setup();
}

void loop() {
  get_imu_data();
  display_imu();
}
