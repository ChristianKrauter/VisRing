// General
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <movingAvg.h> // https://github.com/JChristensen/movingAvg

// PPG
#include "MAX30105.h"
#include "heartRate.h"

// Temperature
#include <Adafruit_TMP117.h>

// IMU
#include "ICM_20948.h" // http://librarymanager/All#SparkFun_ICM_20948_IMU
#define CS_PIN 2

// Bluetooth
#include <bluefruit.h>

class VisRingUtility
{
private:
    // PPG
    MAX30105 ppg;

    static const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
    byte rates[RATE_SIZE];           // Array of heart rates
    byte rateSpot = 0;
    long lastBeat = 0; // Time at which the last beat occurred

    float beatsPerMinute;
    int beatAvg;

    // Temp
    Adafruit_TMP117 tmp117;

    // IMU
    ICM_20948_SPI imu;

    // Timing variables
    unsigned long millisOfLastTap = 0;     // Used to record time of first tap
    const unsigned long delayPeriod = 300; // Forced delay between first and second tap to ensure separate events
    unsigned long displayStartTime = 0;    // Start time of display
    unsigned long turnOffPeriod = 5000;    // Duration before display turns off
    int avgLength = 600;                   // Number of values used for rolling average
    int avgDetectLength = 10;              // Number of values averaged and checked for imu activity
    int count = 0;                         // Tap count
    void clearDataIMU();
    movingAvg avgTotalAcc;

    // Bluetooth
    BLEDis bledis;
    void startAdv(void);

public:
    VisRingUtility(int avgLength, int avgDetectLength, int turnOffPeriod, int delayPeriod);

    // HR
    float dataPPG[2] = {
        -1,
        -1,
    };

    void setupPPG();
    void clearDataPPG();
    void updateDataPPG();

    // Temp
    float dataTemp = -1;
    void setupTMP();
    void updateDataTMP();

    // IMU
    float dataIMU[9] = {
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
    bool enableDisplay = true;   // Determines if display should be on
    unsigned long startMillis;   // Start time of the program
    unsigned long currentMillis; // Used for current time
    void setupIMU();
    void updateDataIMU();
    int getIMUTotalAccInt();
    void turnOffDisplay();
    void detectTap();

    // Bluetooth
    BLEUart bleuart;
    uint32_t rxCount = 0;
    uint32_t rxStartTime = 0;
    uint32_t rxLastTime = 0;
    int COUNT;
    void setupBluetooth();
    void sendBTMessage(const char msg[]);
};
