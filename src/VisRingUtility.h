// General
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <movingAvg.h> // https://github.com/JChristensen/movingAvg

// imu
#include "ICM_20948.h" // http://librarymanager/All#SparkFun_ICM_20948_IMU
#define CS_PIN 2

class VisRingUtility
{
private:
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

public:
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

    // Determines if display should be on
    bool enableDisplay = true;
    // Timing variables
    unsigned long startMillis;   // Start time of the program
    unsigned long currentMillis; // Used for current time

    VisRingUtility(int avgLength, int avgDetectLength, int turnOffPeriod, int delayPeriod);
    void setupIMU();
    void updateDataIMU();
    int getIMUTotalAccInt();
    void turnOffDisplay();
    void detectTap();
};
