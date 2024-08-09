/*
  Control a display extended smart ring
  All Visualization examples run in sequence
*/

#include <SPI.h>
#include <VisRing.h>

// Initialize the display with the follow pin connections
// Note that you should not change SCLK and MOSI because the
// library uses hardware SPI
VisRing VisRing(15, 16, 12, 13);
HardwareSerial *printer = &Serial;

void setup() {
  SPI.begin();
  Serial.begin(115200);
  VisRing.begin(160, 32, printer);  // Display is 160 wide, 32 high
  VisRing.displayGS();
  delay(3000);
  VisRing.clearDisplayGS();
}

void example_HearRate() {
  int hr[] = { 50, 55, 60, 60, 65, 70, 70, 90, 100, 100, 90, 70, 50, 50 };

  for (int variant = 0; variant < 2; variant++) {
    for (int val : hr) {
      VisRing.drawHR(val, variant, 15, 4);
      VisRing.displayGS();
      delay(500);
      VisRing.clearDisplayGS();
    }
  }
}

void example_HearRateZone() {
  int hr[] = { 40, 50, 55, 60, 60, 65, 90, 100, 150, 160, 170, 190, 150, 130, 120, 100, 90, 70, 50, 40 };

  for (int val : hr) {
    VisRing.drawHRZ(val, 15, 4);
    VisRing.displayGS();
    delay(500);
    VisRing.clearDisplayGS();
  }
}

void example_BarChartHor() {
  int values_count = 3;
  int values[values_count] = { 0, 5, 10 };
  VisRing.drawBarChartHor(values, values_count, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_2 = 10;
  int values_2[values_count_2] = { 41, 76, 1, 6, 24, 44, 100, 68, 53, 2 };
  VisRing.drawBarChartHor(values_2, values_count_2, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_3 = 20;
  int values_3[values_count_3] = { 3, 16, 30, 82, 8, 23, 99, 6, 5, 57, 55, 71, 100, 33, 80, 34, 1, 3, 63, 42 };
  VisRing.drawBarChartHor(values_3, values_count_3, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_4 = 40;
  int values_4[values_count_4] = { 57, 9, 32, 47, 39, 45, 59, 41, 100, 60, 66, 70, 69, 46, 67, 69, 75, 85, 6, 14, 15, 61, 5, 18, 49, 47, 80, 69, 48, 96, 55, 18, 84, 52, 36, 86, 53, 62, 22, 90 };
  VisRing.drawBarChartHor(values_4, values_count_4, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_5 = 80;
  int values_5[values_count_5] = { 38, 26, 16, 78, 100, 50, 43, 91, 78, 31, 60, 72, 24, 14, 88, 96, 44, 65, 33, 31, 66, 0, 3, 83, 0, 84, 82, 4, 33, 68, 87, 25, 49, 14, 9, 21, 9, 41, 24, 49, 56, 3, 36, 10, 10, 8, 95, 88, 28, 47, 88, 46, 75, 31, 46, 26, 65, 72, 77, 89, 20, 34, 9, 76, 38, 47, 39, 37, 16, 1, 71, 21, 52, 32, 69, 75, 98, 42, 92, 0 };
  VisRing.drawBarChartHor(values_5, values_count_5, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();
}

void example_BarChartVert() {
  int values_count = 2;
  int values[values_count] = { 5, 10 };
  VisRing.drawBarChartVert(values, values_count, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_2 = 8;
  int values_2[values_count_2] = { 5, 10, 15, 20, 30, 35, 40, 45 };
  VisRing.drawBarChartVert(values_2, values_count_2, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_3 = 16;
  int values_3[values_count_3] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80 };
  VisRing.drawBarChartVert(values_3, values_count_3, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();
}

void example_BarChartVert_SmallMultiples() {
  int values_0[7][16] = { { 55, 88, 2, 5, 23, 49, 83 }, { 39, 95, 64, 56, 11, 55, 75 } };
  int charts_count_0 = 2;
  int values_counts_0[7] = { 7, 7 };
  bool focus_charts_0[7] = { true, false, false, false, false, false, false };
  int value_range_min = 0;
  int value_range_max_0 = 100;

  VisRing.drawBarChartVertSmallMultiples(values_0, charts_count_0, values_counts_0, focus_charts_0, value_range_min, value_range_max_0, 4, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_1[7][16] = { { 64, 44, 20, 67, 28, 11, 66, 26, 70, 76, 30, 22, 21, 37, 82, 36 }, { 72, 69, 52, 46, 29, 59, 73, 25, 48, 87, 89, 5, 43, 35, 60, 38 }, { 97, 24, 74, 50, 39, 88, 2, 9, 4, 15, 71, 34, 96, 27, 91, 40 }, { 12, 100, 99, 77, 18, 83, 19, 54, 78, 80, 92, 62, 42, 85, 93, 68 } };
  int charts_count_1 = 4;
  int values_counts_1[7] = { 16, 16, 16, 16, 0, 0, 0 };
  bool focus_charts_1[7] = { true, true, true, true, false, false, false };
  int value_range_min_1 = 0;
  int value_range_max_1 = 100;

  VisRing.drawBarChartVertSmallMultiples(values_1, charts_count_1, values_counts_1, focus_charts_1, value_range_min_1, value_range_max_1, 4, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_2[7][16] = { { 21, 17, 72, 81, 50 }, { 47, 65, 98, 51, 63 }, { 69, 73, 31, 92, 39 }, { 90, 24, 14, 10, 53 }, { 68, 94, 55, 100, 77 }, { 82, 25, 83, 27, 45 }, { 4, 44, 99, 59, 23 } };
  int charts_count_2 = 7;
  int values_counts_2[7] = { 5, 5, 5, 5, 5, 5, 5 };
  bool focus_charts_2[7] = { false, false, false, true, false, false, false };
  int value_range_min_2 = 0;
  int value_range_max_2 = 100;

  VisRing.drawBarChartVertSmallMultiples(values_2, charts_count_2, values_counts_2, focus_charts_2, value_range_min_2, value_range_max_2, 4, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();
}

void example_lineChart() {
  int values_count = 2;
  int values[values_count] = { 0, 10 };
  VisRing.lineChart(values, values_count, 3);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_2 = 8;
  int values_2[values_count_2] = { 80, 12, 55, 19, 74, 54, 58, 82 };
  VisRing.lineChart(values_2, values_count_2, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_3 = 16;
  int values_3[values_count_3] = { 14, 38, 38, 27, 20, 91, 11, 38, 41, 56, 23, 77, 0, 0, 47, 56 };
  VisRing.lineChart(values_3, values_count_3, 1);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_4 = 40;
  int values_4[values_count_4] = { 29, 51, 34, 44, 90, 44, 12, 16, 75, 0, 47, 57, 70, 1, 45, 49, 36, 74, 73, 30, 70, 8, 71, 22, 11, 37, 49, 83, 37, 40, 30, 36, 25, 50, 48, 7, 91, 64, 26, 64 };
  VisRing.lineChart(values_4, values_count_4, 15);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_5 = 80;
  int values_5[values_count_5] = { 1, 47, 6, 41, 18, 9, 50, 56, 80, 25, 63, 29, 42, 15, 98, 78, 36, 99, 64, 12, 55, 49, 7, 14, 85, 68, 24, 17, 74, 77, 40, 27, 4, 89, 39, 19, 13, 94, 83, 91, 53, 20, 60, 66, 87, 93, 2, 31, 3, 54, 84, 45, 65, 46, 16, 86, 21, 59, 28, 10, 92, 70, 37, 38, 34, 75, 81, 67, 52, 8, 62, 73, 58, 26, 61, 43, 0, 35, 82, 23 };
  VisRing.lineChart(values_5, values_count_5, 12);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();

  int values_count_6 = 160;
  int values_6[values_count_6] = { 89, 58, 98, 73, 79, 80, 10, 7, 25, 45, 27, 59, 34, 94, 23, 5, 46, 95, 84, 75, 33, 63, 57, 28, 75, 93, 71, 52, 93, 31, 92, 33, 65, 6, 67, 53, 35, 64, 66, 30, 25, 92, 53, 69, 60, 68, 90, 63, 2, 37, 72, 75, 27, 68, 91, 17, 29, 16, 78, 94, 51, 32, 73, 8, 28, 16, 61, 19, 63, 17, 34, 71, 64, 39, 70, 55, 48, 53, 49, 42, 57, 22, 63, 4, 48, 30, 18, 16, 37, 3, 88, 80, 99, 52, 71, 48, 27, 0, 94, 13, 75, 33, 33, 96, 65, 36, 16, 20, 52, 50, 54, 95, 55, 68, 0, 74, 79, 48, 18, 38, 5, 41, 7, 35, 11, 26, 20, 18, 5, 9, 65, 96, 34, 27, 21, 28, 61, 83, 6, 95, 41, 89, 7, 69, 59, 61, 92, 4, 44, 99, 12, 47, 15, 84, 29, 18, 75, 45, 83, 84 };
  VisRing.lineChart(values_6, values_count_6, 5);
  VisRing.displayGS();
  delay(500);
  VisRing.clearDisplayGS();
}

void example_radialProgress() {
  for (int i = 0; i <= 8; i++) {
    VisRing.radialProgressChart(80, 15, 15, i, true, 1, 15, 12);
    VisRing.displayGS();
    delay(1000);
    VisRing.clearDisplayGS();
  }
}

void example_Notifications() {
  int len = 9;
  int notification_counts[] = { 1, 2, 3, 4, 4, 5, 5, 5, 6 };

  for (int i = 0; i < len; i++) {
    VisRing.showNotification(notification_counts[i], 10);
    VisRing.displayGS();
    delay(1000);
    VisRing.clearDisplayGS(0);
  }
}

void example_NotificationsIcon() {
  int len = 9;
  int notification_counts[] = { 1, 2, 3, 4, 4, 5, 5, 5, 6 };

  for (int i = 0; i < len; i++) {
    VisRing.showNotificationIcon(notification_counts[i], 10);
    VisRing.displayGS();
    delay(1000);
    VisRing.clearDisplayGS(0);
  }
}

void example_Pattern() {
  for (int i = 0; i < 3; i++) {
    VisRing.drawPattern(i);
    VisRing.displayGS();
    delay(2000);
    VisRing.clearDisplayGS();
  }
}

void example_Text() {
  String text = "Please remember that we are meeting with Lars today at 4 pm at the bus stop. Can you be there at 3:30?";
  VisRing.printStringGS(0, 24, text, false, 15, 0);
  VisRing.displayGS();
  delay(3000);
  VisRing.clearDisplayGS();
}

void loop() {
  example_HearRate();
  example_HearRateZone();
  example_BarChartHor();
  example_BarChartVert();
  example_Notifications();
  example_lineChart();
  example_radialProgress();
  example_NotificationsIcon();
  example_Pattern();
  example_BarChartVert_SmallMultiples();
  example_Text();
}