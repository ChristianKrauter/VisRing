#include <SSD1320_OLED.h>

class VisRing : public SSD1320
{
public:
    using SSD1320::SSD1320;

    void scaleToMinMax(int target_min, int target_max, int values[], int scaled_values[], int values_count);
    void scaleToMinMaxKnownRange(int target_min, int target_max, int value_range_min, int value_range_max, int values[], int scaled_values[], int values_count);

    void drawHeart(int x, int y, int fill_mode, int grayscale);
    void drawEnvelope(int x, int y, int grayscale);
    void drawPattern(int type);
    void drawSlantedBackground(int x0, int x1, int slant_offset, int grayscale);
    void drawHeartBeat(int grayscale);

    void drawBarChartHor(int values[], int values_count, int grayscale);
    void drawBarChartVert(int values[], int values_count, int grayscale);
    void drawBarChartVertSmallMultiples(int values[7][16], int charts_count, int values_counts[7], bool focus_charts[7], int value_range_min, int value_range_max, int grayscale, int grayscale_focus);
    void lineChart(int values[], int values_count, bool fixed_scaling, int value_range_min, int value_range_max, int grayscale);
    void drawCircleSegment(int x0, int y0, int x, int y, int parts, int grayscale);
    void radialProgressChart(int x0, int y0, int radius, int parts, bool show_percentage, int grayscale_background, int grayscale, int grayscale_percentage);

    void showNotification(int number, int grayscale);
    void showNotificationIcon(int number, int grayscale);
    void drawHR(int hr, int variant, int grayscale, int grayscale_background);
    void drawHRZ(int hr, int grayscale, int grayscale_background);
};
