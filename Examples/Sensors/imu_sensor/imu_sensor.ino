/*
  VisRing code to read and display IMU values.
*/

// General
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

// imu
#include "ICM_20948.h"  // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
#define CS_PIN 2        // Which pin you connect CS to. Used only when "USE_SPI" is defined

// oled
#include <VisRing.h>

VisRing VisRing(15, 16, 12, 13);

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

ICM_20948_SPI imu;  // If using SPI create an ICM_20948_SPI object
bool print_acc = true;
bool print_gyr = true;
bool print_mag = true;

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
    VisRing.clearDisplayGS();

    if (print_acc) {
      VisRing.printStringGS(
        10,
        0,
        "acc X:" + (String)data[0] + "  Y:" + (String)data[1] + "  Z:" + (String)data[2],
        false,
        15,
        0);
    }

    if (print_gyr) {
      VisRing.printStringGS(
        10,
        12,
        "gyr X:" + (String)data[3] + "  Y:" + (String)data[4] + "  Z:" + (String)data[5],
        false,
        15,
        0);
    }

    if (print_mag) {
      VisRing.printStringGS(
        10,
        24,
        "mag X:" + (String)data[6] + "  Y:" + (String)data[7] + "  Z:" + (String)data[8],
        false,
        15,
        0);
    }

    VisRing.displayGS();

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

void print_data() {

  if (print_acc) {
    Serial.println("acc X: " + (String)data[0] + "  Y: " + (String)data[1] + "  Z: " + (String)data[2]);
  }

  if (print_gyr) {
    Serial.println("gyr X: " + (String)data[3] + "  Y: " + (String)data[4] + "  Z: " + (String)data[5]);
  }

  if (print_mag) {
    Serial.println("mag X: " + (String)data[6] + "  Y: " + (String)data[7] + "  Z: " + (String)data[8]);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  imu_setup();

  VisRing.begin(160, 32);  // Display is 160 wide, 32 high
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
}

void loop() {
  get_imu_data();
  display_imu();
  delay(1000);
}
