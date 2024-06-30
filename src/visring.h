#include <SSD1320_OLED.h>

class VisRing : public SSD1320 {
    public:
        using SSD1320::SSD1320;

        void scaleToMinMax(int target_min, int target_max, int values[], int values_count);

        void drawHeart(int x, int y, int grayscale);
        void drawEnvelope(int x, int y, int grayscale);
        void drawPattern(int type);

        void drawBarChartHor(int values[], int values_count, int grayscale);
        void drawBarChartVert(int values[], int values_count, int grayscale);
        void lineChart(int values[], int values_count, int grayscale);
        void drawCircleSegment(int x0, int y0, int x, int y, int parts, int grayscale);
        void radialProgressChart(int x0, int y0, int radius, int parts, bool show_percentage, int grayscale_background, int grayscale, int grayscale_percentage);

        void showNotification(int number, int grayscale);
        void showNotificationIcon(int number, int grayscale);
        void drawHR(int hr, int grayscale, int grayscale_heart);
};
