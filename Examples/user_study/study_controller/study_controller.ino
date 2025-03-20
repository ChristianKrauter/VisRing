/*
  VisRing Bluetooth code to send and receive data via BLE UART.
*/

// oled
#include <VisRing.h>
#include <VisRingUtility.h>

VisRing VisRing(15, 16, 12, 13);

// int avgLength, int avgDetectLength, int turnOffPeriod, int delayPeriod
VisRingUtility VisRingUtility(600, 10, 1000, 300);

int condition = 0;

// Timing
unsigned long startMillis = 0;

// Condition Three timing
int durationConThree = 120000;                                  // 2 minutes = 120 seconds
unsigned long progressIntervalConThree = durationConThree / 8;  // Progress chart supports 8 segements
bool timerStartedConThree = false;

// Condition Four timing
int durationConFour = 120000;                                 // 2 minutes = 120 seconds
unsigned long progressIntervalConFour = durationConFour / 8;  // 15 seconds for each bar chart
bool timerStartedConFour = false;

const int MAX_SENSOR_VALUES = 80;  // Size of the list (number of last sensor readings)
int listPPG[MAX_SENSOR_VALUES];    // Array to hold the last `n` sensor readings
int currentIndex = 0;              // Keeps track of the number of values added

// This function simulates receiving a new sensor value
void updateListPPG() {
  if (VisRingUtility.dataPPG[0] != -1) {

    // If the array is not full, just add the new value
    if (currentIndex < MAX_SENSOR_VALUES) {
      listPPG[currentIndex] = VisRingUtility.dataPPG[0];
      currentIndex++;
    } else {
      // Shift all values forward by one position (remove the oldest)
      for (int i = 0; i < MAX_SENSOR_VALUES - 1; i++) {
        listPPG[i] = listPPG[i + 1];
      }
      // Add the new value at the end of the array
      listPPG[MAX_SENSOR_VALUES - 1] = VisRingUtility.dataPPG[0];
    }
  }
}

void conditionOne() {
  VisRing.clearDisplayGS();
  VisRing.drawHRZ(listPPG[MAX_SENSOR_VALUES - 1], 15, 4);
  VisRing.displayGS();
}

void conditionTwo() {
  VisRing.clearDisplayGS();
  VisRing.lineChart(listPPG, MAX_SENSOR_VALUES, true, 0, 200, 15);
  VisRing.displayGS();
}

void conditionThree() {
  VisRing.clearDisplayGS();

  if (!timerStartedConThree) {
    startMillis = millis();
    timerStartedConThree = true;
  } else if (millis() - startMillis > durationConThree) {
    startMillis = millis();
  }

  int parts = floor((millis() - startMillis) / progressIntervalConThree);
  VisRing.radialProgressChart(80, 15, 15, parts, false, 1, 15, 12);
  VisRing.displayGS();
}

void conditionFour() {
  VisRing.clearDisplayGS();

  if (!timerStartedConFour) {
    startMillis = millis();
    timerStartedConFour = true;
  } else {

    //
    // 24 hours
    //
    if (millis() - startMillis < progressIntervalConFour) {
      int values_count_3 = 24;
      int values_3[values_count_3] = { 0, 0, 0, 0, 0, 490, 998, 79, 1447, 1361, 211, 248, 2219, 564, 0, 417, 1182, 1810, 2404, 694, 365, 220, 55, 0 };
      VisRing.drawBarChartHor(values_3, values_count_3, 15);
    }
    //
    // 4 weeks á 7 days
    //
    else if (millis() - startMillis < (progressIntervalConFour * 2)) {
      int values_0[7][16] = {
        { 4368, 1135, 6624, 4727, 713, 8414, 5557 },
        { 4502, 2715, 6857, 5152, 7967, 7617, 7733 },
        { 3244, 3930, 8941, 16431, 5115, 5639, 9910 },
        { 6168, 5510, 14514, 6725, 6688, 8114, 6506 }
      };
      int charts_count_0 = 4;
      int values_counts_0[7] = { 7, 7, 7, 7 };
      bool focus_charts_0[7] = { true, false, false, false, false, false, false };
      int value_range_min = 0;
      int value_range_max_0 = 20000;

      VisRing.drawBarChartVertSmallMultiples(values_0, charts_count_0, values_counts_0, focus_charts_0, value_range_min, value_range_max_0, 4, 15);
    }
    //
    // 7 Days
    //
    else if (millis() - startMillis < (progressIntervalConFour * 3)) {
      int values_count_2 = 7;
      int values_2[values_count_2] = { 4279, 17173, 3052, 13303, 17377, 2487, 7584 };
      VisRing.drawBarChartVert(values_2, values_count_2, 15);
    }
    //
    // 7 days á 16 waking hoursF
    //
    else if (millis() - startMillis < (progressIntervalConFour * 4)) {
      int values_1[7][16] = {
        { 0, 0, 0, 81, 122, 195, 2141, 3397, 1034, 1114, 42, 28, 0, 252, 0, 0 },
        { 0, 26, 46, 150, 57, 11, 128, 0, 196, 1757, 346, 2391, 2742, 21, 98, 10 },
        { 1927, 838, 634, 30, 927, 738, 534, 810, 2219, 1124, 873, 469, 37, 1888, 2810, 330 },
        { 0, 1603, 88, 599, 131, 607, 1421, 118, 288, 1077, 685, 585, 547, 569, 1505, 221 },
        { 19, 2000, 12, 563, 444, 592, 1002, 0, 334, 0, 2167, 1095, 603, 1106, 1210, 2635 },
        { 37, 1846, 316, 2996, 2047, 1656, 2886, 2258, 1240, 829, 0, 1607, 551, 199, 236, 0 },
        { 0, 264, 1219, 0, 933, 2348, 247, 34, 721, 850, 759, 1390, 0, 0, 17, 7 },
      };
      int charts_count_1 = 7;
      int values_counts_1[7] = { 16, 16, 16, 16, 16, 16, 16 };
      bool focus_charts_1[7] = { true, true, false, true, false, false, true };
      int value_range_min_1 = 0;
      int value_range_max_1 = 100;

      VisRing.drawBarChartVertSmallMultiples(values_1, charts_count_1, values_counts_1, focus_charts_1, value_range_min_1, value_range_max_1, 4, 15);
    }
    // All charts shown once, restart
    else {
      startMillis = millis();
    }
  }

  VisRing.displayGS();
}

void handleMessage(uint16_t conn_hdl) {
  (void)conn_hdl;

  VisRingUtility.rxLastTime = millis();

  // first packet
  if (VisRingUtility.rxCount == 0) {
    VisRingUtility.rxStartTime = millis();
  }

  while (VisRingUtility.bleuart.available()) {
    switch ((uint8_t)VisRingUtility.bleuart.read()) {
      case 'a':
        Serial.println("condition 1");
        condition = 1;
        break;
      case 'b':
        Serial.println("condition 2");
        condition = 2;
        break;
      case 'c':
        Serial.println("condition 3");
        condition = 3;
        break;
      case 'd':
        Serial.println("condition 4");
        condition = 4;
        break;
      default:
        break;
    }
  }
}

void connect_callback(uint16_t conn_handle) {
  BLEConnection *conn = Bluefruit.Connection(conn_handle);
  Serial.println("Connected");

  // request PHY changed to 2MB
  Serial.println("Request to change PHY");
  conn->requestPHY();

  // request to update data length
  Serial.println("Request to change Data Length");
  conn->requestDataLengthUpdate();

  // request mtu exchange
  Serial.println("Request to change MTU");
  conn->requestMtuExchange(247);

  // request connection interval of 7.5 ms
  // conn->requestConnectionParameter(6); // in unit of 1.25

  // delay a bit for all the request to complete
  delay(1000);
}

/**
   Callback invoked when a connection is dropped
   @param conn_handle connection where this event happens
   @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
*/
void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  (void)conn_handle;
  (void)reason;
  Serial.print("total count:");
  Serial.println(VisRingUtility.COUNT);
  Serial.println();
  Serial.print("Disconnected, reason = 0x");
  Serial.println(reason, HEX);
}

void bleuart_notify_callback(uint16_t conn_hdl, bool enabled) {
  if (enabled) {
    Serial.println("Send a key and press enter to start test");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  VisRingUtility.setupPPG();

  delay(1000);

  VisRingUtility.setupBluetooth("VisRing #1");

  // Bluetooth callback functions
  Bluefruit.Periph.setConnectCallback(handleMessage);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
  VisRingUtility.bleuart.setNotifyCallback(bleuart_notify_callback);
  VisRingUtility.bleuart.setRxCallback(handleMessage);


  VisRing.begin(160, 32);  // Display is 160 wide, 32 high
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
  Serial.println("started");
}

void loop() {
  if (Bluefruit.connected() && VisRingUtility.bleuart.notifyEnabled()) {

    VisRingUtility.updateDataPPG();
    updateListPPG();

    switch (condition) {
      case 1:
        conditionOne();
        break;
      case 2:
        conditionTwo();
        break;
      case 3:
        conditionThree();
        break;
      case 4:
        conditionFour();
        break;
      default:
        break;
    }
  }
  if (!Bluefruit.connected()) {
    VisRingUtility.COUNT = 0;
  }
  delay(500);
}
