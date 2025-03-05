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
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include "ArduinoJson.h"
#include <SPI.h>

// bluetooth
#include <bluefruit.h>

//////////////////////
/////// general //////
//////////////////////

int total_num_data = 14;

float ALL_MAX[14] = {
  -1, -1, -1,
  -1, -1, -1,
  -1, -1, -1,
  -1, -1, -1,
  -1, -1
};

///////////////////////
////// bluetooth //////
///////////////////////

BLEDis bledis;
BLEUart bleuart;
uint32_t rxCount = 0;
uint32_t rxStartTime = 0;
uint32_t rxLastTime = 0;
int COUNT;

void bluetooth_setup() {
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
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
  Bluefruit.Periph.setConnInterval(6, 12);  // 7.5 - 15 ms
  Bluefruit.setName("VisRing#1");
  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  bleuart.setRxCallback(bleuart_rx_callback);
  bleuart.setNotifyCallback(bleuart_notify_callback);

  // Set up and start advertising
  startAdv();
  Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");
  Serial.println("Once connected, enter character(s) that you wish to send");
}

void send_message(String msg) {
  size_t sent_data_size = bleuart.write("test");
}

void startAdv(void) {
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
  Serial.println(COUNT);
  Serial.println();
  Serial.print("Disconnected, reason = 0x");
  Serial.println(reason, HEX);
}

void bleuart_rx_callback(uint16_t conn_hdl) {
  (void)conn_hdl;

  rxLastTime = millis();

  // first packet
  if (rxCount == 0) {
    rxStartTime = millis();
  }

  while (bleuart.available()) {
    Serial.print((char)bleuart.read());
  }
}

void bleuart_notify_callback(uint16_t conn_hdl, bool enabled) {
  if (enabled) {
    Serial.println("Send a key and press enter to start test");
  }
}

//////////////////
////// main //////
//////////////////

void setup() {
  Serial.begin(115200);
  Wire.setClock(3400000);
  Wire.begin();
  SPI.begin();

  bluetooth_setup();

  Serial.println("Bluefruit52 Throughput Example");
  Serial.println("------------------------------\n");
}

void loop() {
  if (Bluefruit.connected() && bleuart.notifyEnabled()) {
    int msg = rand();
    send_message((String)msg);
  }

  if (!Bluefruit.connected()) {
    COUNT = 0;
  }
}
