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

// ppg
#include "MAX30105.h"
#include "heartRate.h"

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
    -1, -1};

//////////////////////
//////// oled ////////
//////////////////////

SSD1320 flexibleOLED(15, 16, 12, 13); // NRF52832 15 = CS, 16 = RES, 13 = SCLK, 11 = SDIN

void oled_setup()
{
  flexibleOLED.begin(160, 32); // Display is 160 wide, 32 high
  flexibleOLED.clearDisplay();
}

//////////////////////
///// ppg sensor /////
//////////////////////
MAX30105 ppg;

const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];    // Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; // Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void ppg_setup()
{
  Serial.println("MAX30105 Basic Readings Example");

  // Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 0xFF; // Options: 0=Off to 255=50mA
  byte sampleAverage = 1;    // Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3;          // Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 1600;     // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;      // Options: 69, 118, 215, 411
  int adcRange = 16384;      // Options: 2048, 4096, 8192, 16384

  for (byte x = 0; x < RATE_SIZE; x++)
  {
    rates[x] = (byte)70;
  }

  // Initialize sensor
  if (ppg.begin(Wire, I2C_SPEED_FAST, 0x57) == false)
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  ppg.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); // Configure sensor. Use 6.4mA for LED drive
  ppg.setPulseAmplitudeRed(0x50);
  ppg.setPulseAmplitudeIR(0x50);
  ppg.setPulseAmplitudeGreen(0x96);
  ppg.setPulseAmplitudeProximity(0xff);
  Serial.println("MAX30101 Success!");
}

void get_ppg_data(void)
{
  ppg.check();
  while (ppg.available()) // do we have new data?
  {
    ALL_MAX[0] = ppg.getFIFORed();
    ALL_MAX[1] = ppg.getFIFOIR();
    ALL_MAX[2] = ppg.getFIFOGreen();

    ppg.nextSample();
  }
}

int get_hr_data(void)
{
  ppg.check();
  long irValue = ppg.getIR();

  if (checkForBeat(irValue) == true)
  {
    // We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; // Store this reading in the array
      rateSpot %= RATE_SIZE;                    // Wrap variable

      // Take average of readings
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
    return beatAvg;
  }
  return -1;
  // ALL_MAX[0] = irValue;
  // ALL_MAX[1] = beatsPerMinute;
  // ALL_MAX[2] = beatAvg;
}

void display_hr(int value)
{
  if (value != -1)
  {
    flexibleOLED.setFontType(0); // Large font
    flexibleOLED.setCursor(25, 12);
    flexibleOLED.print("HR: ");
    flexibleOLED.print(value);
    flexibleOLED.display();
    Serial.println(value);
  }
}

//////////////////
////// main //////
//////////////////

void setup()
{
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();
  SPI.begin();

  oled_setup();
  ppg_setup();
}

void clear_all_max()
{
  for (int i = 0; i < total_num_data; i++)
  {
    ALL_MAX[i] = -1;
  }
}

void print_all_max()
{
  Serial.print("[");
  for (int i = 0; i < total_num_data; i++)
  {
    Serial.print(ALL_MAX[i]);
    Serial.print(",");
  }
  Serial.println("]");
}

void loop()
{
  display_hr(get_hr_data());
}
