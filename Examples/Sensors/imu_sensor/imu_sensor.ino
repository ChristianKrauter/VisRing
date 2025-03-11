/*
  VisRing code to read and display IMU values.
*/

#include <VisRing.h>
#include <VisRingUtility.h>

bool print_acc = true;
bool print_gyr = true;
bool print_mag = true;

VisRing VisRing(15, 16, 12, 13);
VisRingUtility VisRingUtility;

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  VisRingUtility.setupIMU();

  VisRing.begin(160, 32);  // Display is 160 wide, 32 high
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
}

void displayData() {
  if (VisRingUtility.dataIMU[0] != -1) {
    VisRing.clearDisplayGS();

    if (print_acc) {
      VisRing.printStringGS(
        10,
        0,
        "acc X:" + (String)(int)VisRingUtility.dataIMU[0] + "  Y:" + (String)(int)VisRingUtility.dataIMU[1] + "  Z:" + (String)(int)VisRingUtility.dataIMU[2],
        false,
        15,
        0);
    }

    if (print_gyr) {
      VisRing.printStringGS(
        10,
        12,
        "gyr X:" + (String)(int)VisRingUtility.dataIMU[3] + "  Y:" + (String)(int)VisRingUtility.dataIMU[4] + "  Z:" + (String)(int)VisRingUtility.dataIMU[5],
        false,
        15,
        0);
    }

    if (print_mag) {
      VisRing.printStringGS(
        10,
        24,
        "mag X:" + (String)(int)VisRingUtility.dataIMU[6] + "  Y:" + (String)(int)VisRingUtility.dataIMU[7] + "  Z:" + (String)(int)VisRingUtility.dataIMU[8],
        false,
        15,
        0);
    }

    VisRing.displayGS();
  }
}

void printData(){
  Serial.println(
    "acc X: " + (String)(int)VisRingUtility.dataIMU[0] + "  Y: " + (String)(int)VisRingUtility.dataIMU[1] + "  Z: " + (String)(int)VisRingUtility.dataIMU[2]);
  Serial.println(
    "gyr X: " + (String)(int)VisRingUtility.dataIMU[3] + "  Y: " + (String)(int)VisRingUtility.dataIMU[4] + "  Z: " + (String)(int)VisRingUtility.dataIMU[5]);
  Serial.println(
    "mag X: " + (String)(int)VisRingUtility.dataIMU[6] + "  Y: " + (String)(int)VisRingUtility.dataIMU[7] + "  Z: " + (String)(int)VisRingUtility.dataIMU[8]);
}

void loop() {
  VisRingUtility.updateDataIMU();
  displayData();
  printData();
  delay(1000);
}
