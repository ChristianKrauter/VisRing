#include <VisRing.h>

void VisRing::scaleToMinMax(int target_min, int target_max, int values[], int values_count){
  int values_dif = 0;
  int target_dif = target_max - target_min;
  int values_min = values[0];
  int values_max = values[0];
  int scaled_values[] = {};

  for (int i = 1; i < values_count; i++) {
    if (values[i] > values_max) {
     values_max = values[i];
    }
    if (values[i] < values_min) {
     values_min = values[i];
    }
  }

  values_dif = values_max - values_min;

  for (int i = 0; i < values_count; i++) {
    values[i] = ((target_dif * (values[i] - values_min)) / (values_dif)) + target_min;
  }
}

void VisRing::drawHeart(int x, int y, int grayscale){
  SSD1320::lineGS(x, y-2, x, y-6, grayscale);
  SSD1320::lineGS(x+1, y-1, x+1, y-7, grayscale);
  SSD1320::lineGS(x+2, y, x+2, y-8, grayscale);
  SSD1320::lineGS(x+3, y, x+3, y-9, grayscale);
  SSD1320::lineGS(x+4, y, x+4, y-10, grayscale);
  SSD1320::lineGS(x+5, y-1, x+5, y-11, grayscale);

  SSD1320::lineGS(x+6, y-2, x+6, y-12, grayscale);

  SSD1320::lineGS(x+7, y-1, x+7, y-11, grayscale);
  SSD1320::lineGS(x+8, y, x+8, y-10, grayscale);
  SSD1320::lineGS(x+9, y, x+9, y-9, grayscale);
  SSD1320::lineGS(x+10, y, x+10, y-8, grayscale);
  SSD1320::lineGS(x+11, y-1, x+11, y-7, grayscale);
  SSD1320::lineGS(x+12, y-2, x+12, y-6, grayscale);
}

void VisRing::drawBarChartHor(int values[], int values_count, int grayscale){
  if (values_count > 80 | values_count < 2) {
    return;
  }

  scaleToMinMax(0, SSD1320::getDisplayHeight(), values, values_count);

  int width = (SSD1320::getDisplayWidth() - (values_count - 1)) / values_count;

  for (int i = 0; i < values_count; i++) {
    SSD1320::rectFillGS(i+i*1+i*width, 0, width, values[i], grayscale);
  }
}

void VisRing::drawBarChartVert(int values[], int values_count, int grayscale){
  if (values_count > 16 | values_count < 2) {
    return;
  }

  scaleToMinMax(0, SSD1320::getDisplayWidth(), values, values_count);

  int width = (SSD1320::getDisplayHeight() - (values_count - 1)) / values_count;

  for (int i = 0; i < values_count; i++) {
    SSD1320::rectFillGS(0, i+i*1+i*width, values[i], width, grayscale);
  }
}

void VisRing::lineChart(int values[], int values_count, int grayscale){
  if (values_count > 160 | values_count < 2) {
    return;
  }

  scaleToMinMax(0, SSD1320::getDisplayHeight(), values, values_count);
  int step_size = SSD1320::getDisplayWidth()/(values_count-1);

  for (int i = 0; i < values_count-1; i++) {
    SSD1320::lineGS(i*step_size, values[i], (i+1)*step_size, values[i+1], grayscale);
  }
}

void VisRing::drawHR(int hr, int grayscale, int grayscale_heart){
  String text = String(hr) + " bpm";
  drawHeart(65, 20, grayscale_heart);
  SSD1320::printStringGS(25,10,text,grayscale,0);
}