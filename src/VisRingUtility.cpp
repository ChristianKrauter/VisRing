#include <VisRingUtility.h>

VisRingUtility::VisRingUtility(int avgLength, int avgDetectLength, int turnOffPeriod, int delayPeriod) : avgTotalAcc(avgLength, true)
{
  // IMU
  delayPeriod = delayPeriod;
  turnOffPeriod = turnOffPeriod;
  avgLength = avgLength;
  avgDetectLength = avgDetectLength;
}

///////////////////
/////// IMU ///////
///////////////////

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


///////////////////
//// Bluetooth ////
///////////////////

void VisRingUtility::setupBluetooth() {
  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);  // Check bluefruit.h for supported values
  Bluefruit.Periph.setConnInterval(6, 12);  // 7.5 - 15 ms
  Bluefruit.setName("VisRing#1");
  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();


  // Set up and start advertising
  startAdv();
  Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");
  Serial.println("Once connected, enter character(s) that you wish to send");
}

void VisRingUtility::sendBTMessage(const char msg[]) {
  bleuart.write(msg, strlen(msg));
}

void VisRingUtility::startAdv(void) {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // There is no room for Name in Advertising packet
  // Use Scan response for Name
  Bluefruit.ScanResponse.addName();

  /* Start Advertising
     - Enable auto advertising if disconnected
     - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     - Timeout for fast mode is 30 seconds
     - Start(timeout) with timeout = 0 will advertise forever (until connected)

     For recommended advertising interval
     https://developer.apple.com/library/content/qa/qa1931/_index.html
  */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);  // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);    // number of seconds in fast mode
  Bluefruit.Advertising.start(0);              // 0 = Don't stop advertising after n seconds
}