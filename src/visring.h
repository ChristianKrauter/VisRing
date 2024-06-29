#include <SSD1320_OLED.h>

class VisRing : public SSD1320 {
    public:
        using SSD1320::SSD1320;

        void scaleToMinMax(int target_min, int target_max, int values[], int values_count);

        void drawHeart(int x, int y, int grayscale);

        void drawBarChartHor(int values[], int values_count, int grayscale);
        void drawBarChartVert(int values[], int values_count, int grayscale);
        void lineChart(int values[], int values_count, int grayscale);
        void drawHR(int hr, int grayscale, int grayscale_heart);
};
