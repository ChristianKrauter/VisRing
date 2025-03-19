/*
  VisRing Bluetooth code to send and receive data via BLE UART.
*/

// oled
#include <VisRing.h>
#include <VisRingUtility.h>

VisRing VisRing(15, 16, 12, 13);

// int avgLength, int avgDetectLength, int turnOffPeriod, int delayPeriod
VisRingUtility VisRingUtility(600, 10, 1000, 300);

void bleuart_rx_callback(uint16_t conn_hdl) {
  (void)conn_hdl;

  VisRingUtility.rxLastTime = millis();

  // first packet
  if (VisRingUtility.rxCount == 0) {
    VisRingUtility.rxStartTime = millis();
  }

  VisRing.clearDisplay();
  VisRing.setCursor(10, 12);

  while (VisRingUtility.bleuart.available()) {
    char c = (char)VisRingUtility.bleuart.read();
    Serial.print(c);
    VisRing.print(c);
  }
  VisRing.display();
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

  VisRingUtility.setupBluetooth();

  // Bluetooth callback functions
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
  VisRingUtility.bleuart.setNotifyCallback(bleuart_notify_callback);
  VisRingUtility.bleuart.setRxCallback(bleuart_rx_callback);

  VisRing.begin(160, 32);  // Display is 160 wide, 32 high
  VisRing.displayGS();
  delay(1000);
  VisRing.clearDisplayGS();
}

void loop() {
  if (!Bluefruit.connected()) {
    VisRingUtility.COUNT = 0;
  }

  delay(1000);
}
