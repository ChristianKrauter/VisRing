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
/////// PPG ///////
///////////////////

void VisRingUtility::setupPPG()
{
  Serial.println("MAX30105 Basic Readings Example");

  // Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 0xFF; // Options: 0=Off to 255=50mA
  byte sampleAverage = 1;    // Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3;          // Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 1600;     // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411;      // Options: 69, 118, 215, 411
  int adcRange = 16384;      // Options: 2048, 4096, 8192, 16384

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

void VisRingUtility::clearDataPPG()
{
  for (int i = 0; i < 2; i++)
  {
    dataPPG[i] = -1;
  }
}

void VisRingUtility::updateDataPPG()
{
  clearDataPPG();

  ppg.check();
  long irValue = ppg.getIR();

  if (checkForBeat(irValue) == true)
  {
    // We sensed a beat!
    Serial.println("IR:" + (String)irValue);

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
      dataPPG[0] = beatsPerMinute;
      dataPPG[1] = beatAvg;
    }
  }
}

///////////////////
/////// TMP ///////
///////////////////

void VisRingUtility::setupTMP()
{
  // Try to initialize!
  if (!tmp117.begin())
  {
    Serial.println("Failed to find TMP117 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("TMP117 Success!");
  tmp117.setAveragedSampleCount(TMP117_AVERAGE_1X);
  tmp117.setReadDelay(TMP117_DELAY_0_MS);
}

void VisRingUtility::updateDataTMP()
{
  dataTemp = -1;
  sensors_event_t temp; // create an empty event to be filled
  tmp117.getEvent(&temp);
  dataTemp = temp.temperature;
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

void VisRingUtility::setupBluetooth(const char *ringName)
{
  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);                 // Check bluefruit.h for supported values
  Bluefruit.Periph.setConnInterval(6, 12); // 7.5 - 15 ms
  Bluefruit.setName(ringName);
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

void VisRingUtility::sendBTMessage(const char msg[])
{
  bleuart.write(msg, strlen(msg));
}

void VisRingUtility::startAdv(void)
{
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
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
  Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
}