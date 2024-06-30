#include <VisRing.h>

void VisRing::scaleToMinMax(int target_min, int target_max, int values[], int values_count)
{
  int values_dif = 0;
  int target_dif = target_max - target_min;
  int values_min = values[0];
  int values_max = values[0];
  int scaled_values[] = {};

  for (int i = 1; i < values_count; i++)
  {
    if (values[i] > values_max)
    {
      values_max = values[i];
    }
    if (values[i] < values_min)
    {
      values_min = values[i];
    }
  }

  values_dif = values_max - values_min;

  for (int i = 0; i < values_count; i++)
  {
    values[i] = ((target_dif * (values[i] - values_min)) / (values_dif)) + target_min;
  }
}

void VisRing::scaleToMinMaxKnownRange(int target_min, int target_max, int value_range_min, int value_range_max, int values[], int values_count)
{
  int values_dif = value_range_max - value_range_min;
  int target_dif = target_max - target_min;
  int scaled_values[] = {};

  for (int i = 0; i < values_count; i++)
  {
    values[i] = ((target_dif * (values[i] - value_range_min)) / (values_dif)) + target_min;
  }
}

void VisRing::drawHeart(int x, int y, int grayscale)
{
  SSD1320::lineGS(x, y - 2, x, y - 6, grayscale);
  SSD1320::lineGS(x + 1, y - 1, x + 1, y - 7, grayscale);
  SSD1320::lineGS(x + 2, y, x + 2, y - 8, grayscale);
  SSD1320::lineGS(x + 3, y, x + 3, y - 9, grayscale);
  SSD1320::lineGS(x + 4, y, x + 4, y - 10, grayscale);
  SSD1320::lineGS(x + 5, y - 1, x + 5, y - 11, grayscale);

  SSD1320::lineGS(x + 6, y - 2, x + 6, y - 12, grayscale);

  SSD1320::lineGS(x + 7, y - 1, x + 7, y - 11, grayscale);
  SSD1320::lineGS(x + 8, y, x + 8, y - 10, grayscale);
  SSD1320::lineGS(x + 9, y, x + 9, y - 9, grayscale);
  SSD1320::lineGS(x + 10, y, x + 10, y - 8, grayscale);
  SSD1320::lineGS(x + 11, y - 1, x + 11, y - 7, grayscale);
  SSD1320::lineGS(x + 12, y - 2, x + 12, y - 6, grayscale);
}

void VisRing::drawEnvelope(int x, int y, int grayscale)
{
  SSD1320::lineGS(x, y, x, y + 10, grayscale);
  SSD1320::lineGS(x, y, x + 20, y, grayscale);
  SSD1320::lineGS(x, y + 10, x + 20, y + 10, grayscale);
  SSD1320::lineGS(x + 20, y, x + 20, y + 10, grayscale);
  SSD1320::lineGS(x, y + 10, x + 10, y + 5, grayscale);
  SSD1320::lineGS(x + 20, y + 10, x + 10, y + 5, grayscale);
}

void VisRing::drawPattern(int type)
{
  switch (type)
  {
  case 0:
    for (int i = 0; i <= SSD1320::getDisplayWidth() / 10; i++)
    {
      SSD1320::lineGS(i * 10, 0, (i + 1) * 10, 30, i % 14 + 1);
      SSD1320::lineGS((i + 1) * 10, 0, i * 10, 30, (16 - i) % 14 + 1);
    }
    break;
  case 1:
    for (int i = 0; i < SSD1320::getDisplayHeight() / 2; i++)
    {
      SSD1320::lineGS(0, i, 160, i, i);
      SSD1320::lineGS(0, i + 15, 160, i + 15, (16 - i));
    }
    break;
  case 2:
    for (int i = 0; i < 25; i++)
    {
      SSD1320::circleGS(random(0, 160), random(0, 32), random(2, 15), random(0, 15));
    }
    break;
  }
}

void VisRing::drawBarChartHor(int values[], int values_count, int grayscale)
{
  if (values_count > 80 | values_count < 2)
  {
    return;
  }

  scaleToMinMax(0, SSD1320::getDisplayHeight(), values, values_count);

  int width = (SSD1320::getDisplayWidth() - (values_count - 1)) / values_count;

  for (int i = 0; i < values_count; i++)
  {
    SSD1320::rectFillGS(i + i * 1 + i * width, 0, width, values[i], grayscale);
  }
}

void VisRing::drawBarChartVert(int values[], int values_count, int grayscale)
{
  if (values_count > 16 | values_count < 2)
  {
    return;
  }

  scaleToMinMax(0, SSD1320::getDisplayWidth(), values, values_count);

  int width = (SSD1320::getDisplayHeight() - (values_count - 1)) / values_count;

  for (int i = 0; i < values_count; i++)
  {
    SSD1320::rectFillGS(0, i + i * 1 + i * width, values[i], width, grayscale);
  }
}

void VisRing::drawBarChartVertSmallMultiples(int values[7][16], int charts_count, int values_counts[7], bool focus_charts[7], int value_range_min, int value_range_max, int grayscale, int grayscale_focus)
{

  int height = (SSD1320::getDisplayWidth() - ((charts_count - 1) * 10)) / charts_count;

  for (int chart = 0; chart < charts_count; chart++)
  {
    if (values_counts[chart] < 2)
    {
      return;
    }

    scaleToMinMaxKnownRange(0, height, value_range_min, value_range_max, values[chart], values_counts[chart]);

    int width = (SSD1320::getDisplayHeight() - (values_counts[chart] - 1)) / values_counts[chart];

    int gs = 0;
    if (focus_charts[chart])
    {
      gs = grayscale_focus;
    }
    else
    {
      gs = grayscale;
    }

    for (int i = 0; i < values_counts[chart]; i++)
    {
      SSD1320::lineGS(height * chart + 10 * chart, 0, height * chart + 10 * chart, 32, gs);
      SSD1320::rectFillGS(height * chart + 10 * chart, i + i * 1 + i * width, values[chart][i], width, gs);
    }
  }
}

void VisRing::lineChart(int values[], int values_count, int grayscale)
{
  if (values_count > 160 | values_count < 2)
  {
    return;
  }

  scaleToMinMax(0, SSD1320::getDisplayHeight(), values, values_count);
  int step_size = SSD1320::getDisplayWidth() / (values_count - 1);

  for (int i = 0; i < values_count - 1; i++)
  {
    SSD1320::lineGS(i * step_size, values[i], (i + 1) * step_size, values[i + 1], grayscale);
  }
}

void VisRing::drawCircleSegment(int x0, int y0, int x, int y, int parts, int grayscale)
{
  if (parts >= 1)
  {
    SSD1320::setPixelGS(x0 + x, y0 + y, grayscale);
  }

  if (parts >= 2)
  {
    SSD1320::setPixelGS(x0 + y, y0 + x, grayscale);
  }

  if (parts >= 3)
  {
    SSD1320::setPixelGS(x0 + y, y0 - x, grayscale);
  }

  if (parts >= 4)
  {
    SSD1320::setPixelGS(x0 + x, y0 - y, grayscale);
  }

  if (parts >= 5)
  {
    SSD1320::setPixelGS(x0 - x, y0 - y, grayscale);
  }

  if (parts >= 6)
  {
    SSD1320::setPixelGS(x0 - y, y0 - x, grayscale);
  }

  if (parts >= 7)
  {
    SSD1320::setPixelGS(x0 - y, y0 + x, grayscale);
  }

  if (parts >= 8)
  {
    SSD1320::setPixelGS(x0 - x, y0 + y, grayscale);
  }
}

void VisRing::radialProgressChart(int x0, int y0, int radius, int parts, bool show_percentage, int grayscale_background, int grayscale, int grayscale_percentage)
{
  int d = 3 - 2 * radius;
  int x = 0;
  int y = radius;

  SSD1320::drawCircleGS(x0, y0, x, y, grayscale_background);
  drawCircleSegment(x0, y0, x, y, parts, grayscale);

  while (x <= y)
  {
    x++;

    if (d > 0)
    {
      y--;
      d = d + 4 * (x - y) + 10;
    }
    else
    {
      d = d + 4 * x + 6;
    }

    SSD1320::drawCircleGS(x0, y0, x, y, grayscale_background);
    drawCircleSegment(x0, y0, x, y, parts, grayscale);
  }

  if (show_percentage)
  {
    int percentage = int(parts * 12.5);
    int x = 0;

    if (percentage < 10)
    {
      x = x0 - 2;
    }
    else if (percentage < 100)
    {
      x = x0 - 5;
    }
    else
    {
      x = x0 - 8;
    }

    SSD1320::printStringGS(x, y0 - 3, String(percentage), grayscale_percentage, 0);
  }
}

void VisRing::showNotification(int number, int grayscale)
{
  String text = String(number);

  if (number == 1)
  {
    text += " notification";
  }
  else
  {
    text += " notifications";
  }

  SSD1320::printStringGS(25, 10, text, grayscale, 0);
}

void VisRing::showNotificationIcon(int number, int grayscale)
{
  SSD1320::printStringGS(85, 5, String(number), grayscale, 1);
  drawEnvelope(60, 9, grayscale);
}

void VisRing::drawHR(int hr, int grayscale, int grayscale_heart)
{
  String text = String(hr) + " bpm";
  drawHeart(100, 20, grayscale_heart);
  SSD1320::printStringGS(55, 10, text, grayscale, 0);
}