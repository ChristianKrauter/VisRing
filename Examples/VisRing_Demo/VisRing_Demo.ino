#include <bluefruit.h>
#include "SSD1320_OLED.h"
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <TimeLib.h>
// #include "msg.h"

#define DATA_POINTS 60
#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 32
#define WIDTH 160
#define HEIGHT 32

uint8_t mode = 0;
uint32_t irBuffer[DATA_POINTS];
uint8_t drawBuffer[DATA_POINTS];
// SSD1320: CS, RST, SCLK, MOSI (adjust pins as needed)
SSD1320 display(15, 16, 12, 13);
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
String msg;
bool msgflag = 0;
MAX30105 particleSensor;
// BLE UART service
BLEUart bleuart;
BLEDis bledis;
BLEDfu bledfu; // DFU Service instance

// uint8_t icon_hb[] = {
//   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0x3F, 0xE0, 0x18, 0x07,
//   0xC7, 0x81, 0xE3, 0xCF, 0xE7, 0xF9, 0x9F, 0xE7, 0xF9, 0xBF, 0xFF, 0xFD,
//   0xBF, 0x7F, 0xFC, 0xBE, 0x7F, 0xFF, 0xBC, 0x3F, 0xFF, 0x80, 0x20, 0x01,
//   0x81, 0x20, 0x01, 0xFF, 0x8F, 0xF3, 0xFF, 0x8F, 0xE7, 0xF3, 0x9F, 0xC7,
//   0xF1, 0x9F, 0x8F, 0xF8, 0xFF, 0x1F, 0xFE, 0x7E, 0x3F, 0xFF, 0x1C, 0xFF,
//   0xFF, 0x81, 0xFF, 0xFF, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
// };

uint8_t icon_hb[] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xFF, 0xFF, 0x81, 0xFF,
  0xFF, 0x1C, 0xFF, 0xFE, 0x7E, 0x3F, 0xF8, 0xFF, 0x1F, 0xF1, 0x9F, 0x8F,
  0xF3, 0x9F, 0xC7, 0xFF, 0x8F, 0xE7, 0xFF, 0x8F, 0xF3, 0x81, 0x20, 0x01,
  0x80, 0x20, 0x01, 0xBC, 0x3F, 0xFF, 0xBE, 0x7F, 0xFF, 0xBF, 0x7F, 0xFC,
  0xBF, 0xFF, 0xFD, 0x9F, 0xE7, 0xF9, 0xCF, 0xE7, 0xF9, 0xC7, 0x81, 0xE3,
  0xE0, 0x18, 0x07, 0xFC, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

// === BLE connection callbacks ===
void onConnect(uint16_t connHandle) {
  Serial.println("BLE connected");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setFontType(0);  // 8x16 font (if supported)
  display.print("Connected!");
  display.display();
}

void onDisconnect(uint16_t connHandle, uint8_t reason) {
  Serial.println("BLE disconnected");
  if (mode == 1){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setFontType(0);
    display.print("Waiting for BLE...");
    display.display();
  }
  // Restart advertising
  Bluefruit.Advertising.start(0);
}

void setup() {
  SPI.begin();
  Wire.begin();
  Serial.begin(115200);
  // delay(2000);
  Serial.println("Boot");
  
  if (!particleSensor.begin(Wire)) {
    Serial.println("MAX30105 not found. Please check wiring.");
    while (1);
  }
  // delay(1000);
  // Serial.println("BLE UART + OLED Display");
  particleSensor.setup();  // Use default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  // Init OLED
  display.begin(160, 32);
  display.setContrast(64);              // Adjust brightness
  // display.flipHorizontal(true);          // Rotate 180°
  // display.flipVertical(true);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setFontType(0);
  display.print("Waiting for BLE...");
  display.display();

  // Init BLE
  Bluefruit.begin();
  Bluefruit.setName("VisRing");
  bledis.setManufacturer("OmniLab");
  bledis.setModel("VisRing");
  // Set BLE callbacks
  Bluefruit.Periph.setConnectCallback(onConnect);
  Bluefruit.Periph.setDisconnectCallback(onDisconnect);

  // Init BLE UART
  bleuart.begin();
  bledfu.begin();
  bledis.begin();

  // Advertise
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.Advertising.addService(bledfu);  // DFU advertised
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.start(0);  // 0 = advertise forever

}

void loop() {
  if (bleuart.available()){
      msg = bleuart.readString();
      msg.trim();  // Remove \n or \r if present
      msgflag = 1;
      Serial.print("Received: ");
      Serial.println(msg);
      if (msg.startsWith("mode")){
        parseMode(msg);
      }
      else if (msg.startsWith("time")){
        parseTime(msg);
      }
      else if (msg.startsWith("bri")){
        parseBrightness(msg);
      }
  }
  if (mode == 1){
    if (msgflag) {
      displayMessage(msg);
      msgflag = 0;
    }
  }
  else if (mode == 2){
    uint32_t irValue = particleSensor.getIR();
    if (checkForBeat(irValue) == true)
    {
      long delta = millis() - lastBeat;
      lastBeat = millis();
      beatsPerMinute = 60 / (delta / 1000.0);
      if (beatsPerMinute < 255 && beatsPerMinute > 20)
      {
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }
    for (int i = 0; i < DATA_POINTS - 1; i++) {
      irBuffer[i] = irBuffer[i + 1];
    }
    irBuffer[DATA_POINTS - 1] = irValue;

    uint32_t minVal = irBuffer[0], maxVal = irBuffer[0];
    for (int i = 1; i < DATA_POINTS; i++) {
      if (irBuffer[i] < minVal) minVal = irBuffer[i];
      if (irBuffer[i] > maxVal) maxVal = irBuffer[i];
    }
    uint32_t range = maxVal - minVal;
    if (range == 0) range = 1;

    for (int i = 0; i < DATA_POINTS; i++) {
      drawBuffer[i] = (uint8_t)(((uint64_t)(irBuffer[i] - minVal) * (DISPLAY_HEIGHT - 1)) / range);
    }

    display.clearDisplay(CLEAR_BUFFER);
    uint8_t prevY = 0;
    bool first = true;

    for (int x = 0; x < DISPLAY_WIDTH; x++) {
      float idx = ((float)x / (DISPLAY_WIDTH - 1)) * (DATA_POINTS - 1);
      int i = (int)idx;
      float frac = idx - i;

      uint8_t y1 = drawBuffer[i];
      uint8_t y2 = (i < DATA_POINTS - 1) ? drawBuffer[i + 1] : drawBuffer[i];
      uint8_t y = (uint8_t)((1.0 - frac) * y1 + frac * y2);
      uint8_t yPos = DISPLAY_HEIGHT - 1 - y;

      if (!first) {
        display.line(x - 1, prevY, x, yPos);
      } else {
        first = false;
      }

    prevY = yPos;
  }
    if (beatAvg<100){
    display.rectFill(0, 0, 56, 16, BLACK, NORM);
    }
    else{
    display.rectFill(0, 0, 64, 16, BLACK, NORM);
    }

    display.setCursor(0, 0);
    display.setFontType(1);  // Optional: use smaller font
    display.print("BPM:");
    if (irValue > 50000)
    display.print(beatAvg);
    display.display();
    // delay(10);
  }
  else if (mode == 3){
    drawTime();
    delay(10);
  }
  else if (mode == 4){
    drawEnvelopeNum(3);
  }
  else if (mode == 5){
    putTextFull();
    delay(1000);
  }
  else if (mode == 6){
    putTextFull2();
    delay(1000);
  }
  else if (mode == 7){
    dispDual();
    delay(1000);
  }
}

void parseMode(const String& msg){
  // if (msg.startsWith("mode")) {
    msgflag = 0;
    String numberPart = msg.substring(4);  // Get text after "mode"
    int newMode = numberPart.toInt();      // Extract integer
    if (newMode >= 0) {                    // Optional: check range
      mode = newMode;
      display.clearDisplay(CLEAR_BUFFER);
      display.display();
    }
  // }
}

void parseTime(const String& msg) {
    // String timePart = msg.substring(4, 18);
    int hh, mm, ss, DD, MM, YY;
    DD = 1;
    MM = 1;
    YY = 1970;
    if (msg.length() >= 8){
      hh = msg.substring(4, 6).toInt();
      mm = msg.substring(6, 8).toInt();
    }
    if (msg.length() >= 10){
      ss = msg.substring(8, 10).toInt();
    }
    if (msg.length() == 18){
      DD = msg.substring(10, 12).toInt();
      MM = msg.substring(12, 14).toInt();
      YY = msg.substring(14, 18).toInt();
    }
    Serial.println("Time updated");
    if (hh >= 0 && hh < 24 && mm >= 0 && mm < 60 && ss >= 0 && ss < 60) {
      // If you're using a real RTC or time library, set it here:
      setTime(hh, mm, ss, DD, MM, YY);
    }
    msgflag = 0;
}

void parseBrightness(const String& msg){
  int bri = msg.substring(3, -1).toInt();
  if (bri > 1 && bri < 127){
    display.setContrast(bri);
  }
}

// Display text message on OLED
void displayMessage(const String& message) {
  display.clearDisplay(CLEAR_BUFFER);
  String msg1 = getLine(message, 0);
  String msg2 = getLine(message, 1);
  drawEnvelope2Line(msg1, msg2);
  delay(5000);
  display.clearDisplay(CLEAR_BUFFER);
  display.display();
}

void drawEnvelope(int x, int y, int sx, int sy){
  // display.clearDisplay(CLEAR_BUFFER);
  // display.setColor(15);
  display.line(x-(sx/2), y-(sy/2), x-(sx/2), y+(sy/2));
  display.line(x-(sx/2), y-(sy/2), x+(sx/2), y-(sy/2));
  display.line(x-(sx/2), y+(sy/2), x+(sx/2), y+(sy/2));
  display.line(x+(sx/2), y-(sy/2), x+(sx/2), y+(sy/2)+1);
  display.line(x-(sx/2), y+(sy/2), x, y);
  display.line(x+(sx/2), y+(sy/2), x, y);
  // display.display();
}

void drawEnvelope2Line(String msg1, String msg2){
  for (int i=5; i<15; i+=2){
    display.clearDisplay(CLEAR_BUFFER);
    drawEnvelope(50, 16, i*2, i);
    display.display();
    // delay(5);
  }
  for (int i=15; i>9; i--){
    display.clearDisplay(CLEAR_BUFFER);
    drawEnvelope(50, 16, i*2, i);
    display.display();
    // delay(5);
  }
  delay(500);
  for (int i=0; i<24; i++){
    float t = (float)i / 24;
    float eased = 1 - pow(1 - t, 3);  // Cubic ease-out

    int y = 0 + 24 * eased;
    int y2 = y - 15;
    display.clearDisplay(CLEAR_BUFFER);
    drawEnvelope(50, 16, 20, 10);
    display.setFontType(0);  // 8x16 font
    display.setColor(WHITE);
    if (y <= 18){
      display.setCursor(70, y);
      display.print(msg1);
    }
    else{
      display.setCursor(70, 18);
      display.print(msg1);
    }
    if (y2 >= 0){
      display.setCursor(70, y2);
      display.print(msg2);
    }
    display.display();
    delay(10);
  }
}

void drawEnvelopeNum(int num){
  for (int i=5; i<15; i+=2){
    display.clearDisplay(CLEAR_BUFFER);
    drawEnvelope(50, 16, i*2, i);
    display.display();
    // delay(5);
  }
  for (int i=15; i>9; i--){
    display.clearDisplay(CLEAR_BUFFER);
    drawEnvelope(50, 16, i*2, i);
    display.display();
    // delay(5);
  }
  delay(300);
  display.clearDisplay(CLEAR_BUFFER);
  drawEnvelope(50, 16, 20, 10);
  display.setFontType(1);  // 8x16 font
  display.setColor(WHITE);
  display.setCursor(70, 8);
  display.print(num);
  display.display();
  delay(5000);
}

void drawTime(){
  display.clearDisplay(CLEAR_BUFFER);
  char buf[9];
  sprintf(buf, "%02d:%02d:%02d", hour(), minute(), second());
  display.setCursor(35, 8);
  display.setFontType(2);  // 8x16 font (if supported)
  display.print(buf);
  display.rect(61, 10, 2, 2);
  display.rect(61, 20, 2, 2);
  display.rect(94, 10, 2, 2);
  display.rect(94, 20, 2, 2);
  display.display();
}

String getLine(const String& input, int lineNumber) {
  int start = 0;
  int line = 0;
  int end = input.indexOf('\n');

  while (end != -1) {
    if (line == lineNumber) {
      return input.substring(start, end);
    }
    line++;
    start = end + 1;
    end = input.indexOf('\n', start);
  }

  // If looking for the last line (no \n at end)
  if (line == lineNumber && start < input.length()) {
    return input.substring(start);
  }

  return ""; // Not found
}

void putTextFull(){
  String s = "The Quick Brown Fox Jumps";
  display.clearDisplay(CLEAR_BUFFER);
  display.setFontType(0);  // 8x16 font
  display.setColor(WHITE);
  display.setCursor(0, 10);
  display.print(s);
  display.display();
}

void putTextFull2(){
  String s = "Over the Lazy Dog";
  display.clearDisplay(CLEAR_BUFFER);
  display.setFontType(0);  // 8x16 font
  display.setColor(WHITE);
  display.setCursor(0, 10);
  display.print(s);
  display.display();
}

void dispDual(){
  display.clearDisplay(CLEAR_BUFFER);
  display.drawBitmap(icon_hb, 0, 4, 24, 24);
  display.setFontType(1);  // 8x16 font
  display.setColor(WHITE);
  display.setCursor(26, 10);
  display.print("72");
  display.setFontType(0);  // 8x16 font
  display.setCursor(26, 4);
  display.print("BPM"); 
  display.setFontType(1);  // 8x16 font
  display.setCursor(116, 6);
  display.print("10:24"); 
  display.display();
}