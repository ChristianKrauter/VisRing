#include <VisRingUtility.h>

VisRingUtility::VisRingUtility(int avgLength, int avgDetectLength, int turnOffPeriod, int delayPeriod) : avgTotalAcc(avgLength, true)
{
  // IMU
  delayPeriod = delayPeriod;
  turnOffPeriod = turnOffPeriod;
  avgLength = avgLength;
  avgDetectLength = avgDetectLength;
}

// IMU

int VisRingUtility::getIMUTotalAccInt()
{
  if (imu.dataReady())
  {
    imu.getAGMT();
    return abs(imu.accX() * 0.01f + imu.accY() * 0.01f + imu.accZ() * 0.01f);
  }
  else
  {
    return -1;
  }
}

void VisRingUtility::detectTap()
{
  currentMillis = millis();
  int acc = getIMUTotalAccInt();

  if (acc != -1)
  {
    int avgLong = avgTotalAcc.reading(acc);
    if (currentMillis - millisOfLastTap > delayPeriod)
    {
      int avgShort = avgTotalAcc.getAvg(avgDetectLength);
      if (avgShort > avgLong + 5)
      {
        count++;
        millisOfLastTap = millis();
        if (count > 1)
        {
          count = 0;
          enableDisplay = true;
          displayStartTime = millis();
        }
      }
    }
  }
}

void VisRingUtility::turnOffDisplay()
{
  currentMillis = millis();
  if (currentMillis > displayStartTime)
  {
    if (currentMillis - displayStartTime >= turnOffPeriod)
    {
      enableDisplay = false;
    }
  }
}

void VisRingUtility::updateDataIMU()
{
  clearDataIMU();

  if (imu.dataReady())
  {
    imu.getAGMT();                   // The values are only updated when you call 'getAGMT'
    dataIMU[0] = imu.accX() * 0.01f; // acceleration is measured in m/s^2
    dataIMU[1] = imu.accY() * 0.01f;
    dataIMU[2] = imu.accZ() * 0.01f;
    dataIMU[3] = imu.gyrX(); // gyro is measured in radians/s
    dataIMU[4] = imu.gyrY();
    dataIMU[5] = imu.gyrZ();
    dataIMU[6] = imu.magX(); // magnetometer is measured in uT
    dataIMU[7] = imu.magY();
    dataIMU[8] = imu.magZ();
  }
}

void VisRingUtility::clearDataIMU()
{
  for (int i = 0; i < 9; i++)
  {
    dataIMU[i] = -1;
  }
}

void VisRingUtility::setupIMU()
{
  // imu.enableDebugging();  // Uncomment this line to enable helpful debug messages on Serial

  avgTotalAcc.begin();

  bool initialized = false;
  while (!initialized)
  {

    imu.begin(CS_PIN, SPI);

    imu.startupDefault(false); // Force a full - not minimal - startup
    Serial.print(F("Initialization of the sensor returned: "));
    Serial.println(imu.statusString());
    if (imu.status != ICM_20948_Stat_Ok)
    {
      Serial.println("Trying again...");
      delay(500);
    }
    else
    {
      initialized = true;
    }
  }
  Serial.println("ICM20948 Success!");
}
