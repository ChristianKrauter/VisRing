/*
  VisRing code to read and display heart rate values.
*/
// General
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>

// ppg
#include "MAX30105.h"
#include "heartRate.h"

// oled
#include <VisRing.h>

VisRing VisRing(15, 16, 12, 13);

float data[2] = {
  -1,
  -1,
};

MAX30105 ppg;

const byte RATE_SIZE = 4;  // Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];     // Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;  // Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void ppg_setup() {
  Serial.println("MAX30105 Basic Readings Example");

  // Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 0xFF;  // Options: 0=Off to 255=50mA
  byte sampleAverage = 1;     // Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3;           // Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 1600;      // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;       // Options: 69, 118, 215, 411
  int adcRange = 16384;       // Options: 2048, 4096, 8192, 16384

  // Initialize sensor
  if (ppg.begin(Wire, I2C_SPEED_FAST, 0x57) == false) {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  ppg.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);  // Configure sensor. Use 6.4mA for LED drive
  ppg.setPulseAmplitudeRed(0x50);
  ppg.setPulseAmplitudeIR(0x50);
  ppg.setPulseAmplitudeGreen(0x96);
  ppg.setPulseAmplitudeProximity(0xff);
  Serial.println("MAX30101 Success!");
}

void clear_data() {
  for (int i = 0; i < 2; i++) {
    data[i] = -1;
  }
}

void get_hr_data() {
  clear_data();

  ppg.check();
  long irValue = ppg.getIR();

  if (checkForBeat(irValue) == true) {
    // We sensed a beat!
    Serial.println("IR:" + (String)irValue);

    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;  // Store this reading in the array
      rateSpot %= RATE_SIZE;                     // Wrap variable

      // Take average of readings
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
    data[0] = beatsPerMinute;
    data[1] = beatAvg;
  }
}

void display_hr() {
  if (data[0] != -1) {
    VisRing.clearDisplayGS();

    VisRing.printStringGS(
      10,
      12,
      "BPM:" + (String)(int)data[0] + "  AVG:" + (String)(int)data[1],
      false,
      15,
      0);

    VisRing.displayGS();
    print_data();
  }
}

void print_data() {
  Serial.println("BPM:" + (String)data[0] + "  AVG:" + (String)data[1]);
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  ppg_setup();

  VisRing.begin(160, 32);
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
}

void loop() {
  get_hr_data();
  display_hr();
}
