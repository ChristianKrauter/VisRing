# VisRing
Development code for VisRing project as part of the publication [VisRing: A Display-Extended Smartring for Nano Visualizations](https://doi.org/10.1145/3746059.3747806).

Based on [SparkFun_SSD1320_OLED_Arduino_Library](https://github.com/sparkfun/SparkFun_SSD1320_OLED_Arduino_Library) and using [movingAvg.h](https://github.com/JChristensen/movingAvg) both published under GPL v3.

This code is published under a GPL v3 license.

# More Information
- Project page: https://www.cse.psu.edu/~mkg31/projects/visring/
- Publications
    - [VisRing: A Display-Extended Smartring for Nano Visualizations](https://doi.org/10.1145/3746059.3747806).
    - [Demonstration of VisRing: A Display-Extended Smartring for Nano Visualizations](https://doi.org/10.1145/3746058.3758997)
- Video: https://www.youtube.com/watch?v=FBm_dOg69tY

# Cite as
Taiting Lu, Christian Krauter, Runze Liu, Mara Schulte, Alexander Achberger, Tanja Blascheck, Michael Sedlmair, and Mahanth Gowda. 2025. VisRing: A Display-Extended Smartring for Nano Visualizations. In Proc. Symposium User Interface Software and Technology (UIST). ACM, Article 111, 1–18. https://doi.org/10.1145/3746059.3747806


# Changes to the SSD1320_OLED_Arduino_Library
Besides loading bitmap images directly, none of the functions in the original library support grayscale but only monochrome drawing, severely limiting the possibilities for visualizations.
We overcome this limitation to fully use the display's capabilities by introducing a 2,560-byte array and grayscale variations of all functions while keeping the original functionality.
The library works through a central memory array, which is used to store the next frame.
The functions add their changes there.
The size of this array is limited to 640 bytes due to the memory capacity of past MCUs; thus, one bit controls each pixel, resulting in the limitation of available grayscale values.
By adding a bigger array with accompanying functions, we improve the color range from 1-bit to 4-bit grayscale, the maximum the bendable OLED display supports.

# Dependencies
- [Arduino](https://github.com/arduino/ArduinoCore-avr) 
    - Arduino.h
    - SPI.h
    - Wire.h
- Adafruit
    - [Adafruit_Sensor.h](https://github.com/adafruit/Adafruit_Sensor)
    - [Adafruit_TMP117.h](https://github.com/adafruit/Adafruit_TMP117)
    - [bluefruit.h](https://github.com/adafruit/Adafruit_nRF52_Arduino/tree/master)
- Sparkfun
    - [SparkFun_SSD1320_OLED_Arduino_Library](https://github.com/sparkfun/SparkFun_SSD1320_OLED_Arduino_Library)
    - [MAX30105.h](https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library)
        - heartRate.h
    - [ICM_20948.h](https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary)
- [movingAvg.h](https://github.com/JChristensen/movingAvg)

# Content
## examples
### grayscale_visualizations
All of these functions use mock-up data.
- [all_vis_showcase](./examples/grayscale_visualizations/all_vis_showcase/)
    - Cycles through all visualizations
- [bar_chart_hor](./examples/grayscale_visualizations/bar_chart_hor/)
    - Drawing horizontal bar charts. (i.e., bars next to each other along the long edge of the display)
    - A bar chart needs at least 1 pixel bar width plus 1 pixel margin between bars to be drawn. This leads to a maximum number of 80 data points considering the 160 pixels of the display width. 
- [bar_chart_vert](./examples/grayscale_visualizations/bar_chart_vert/)
    - Drawing vertical bar charts. (i.e., bars next to each other along the short edge of the display)
    - For the vertical bar chart, the maximum number of datapoints is 16 due to the 32 pixels of the display width. 
- [bar_chart_vert_small_multiples](./examples/grayscale_visualizations/bar_chart_vert_small_multiples/)
    - Drawing multiple vertical bar charts. (i.e., charts stacked on top of each other along the long edge and the bars of each chart along the short edge of the display)
    - Small multiples are a technique to show a set of data split into multiple small visualizations of the same type. For example, one can show data for one week as seven bar charts. This library is currently limited to 7 individual charts stacked on top of each other. 
- [heart_rate](./examples/grayscale_visualizations/heart_rate/)
    - Display the heart rate and a heart symbol with an optional slanted background.
- [heart_rate_over_time](./examples/grayscale_visualizations/heart_rate_over_time/)
    - Display the heart rate over time as a linechart.
- [heart_rate_zone](./examples/grayscale_visualizations/heart_rate_zone/)
    - Display a heart which is unfilled, half-filled, or filled based on the heart-rate zone.
- [line_chart](./examples/grayscale_visualizations/line_chart/)
    - Display a line chart.
- [notification](./examples/grayscale_visualizations/notification/)
    - Display a number with text.
- [notification_icon](./examples/grayscale_visualizations/notification_icon/)
    - Displaying a number with an envelope symbol.
- [pattern](./examples/grayscale_visualizations/pattern/)
    - Display one of three decorative patterns.
- [radial_progress](./examples/grayscale_visualizations/radial_progress/)
    - Display a radial progress chart.
- [text](./examples/grayscale_visualizations/text/)
    - Display text.
### sensors
- [bluetooth_code](./Examples/Sensors/bluetooth_code/)
    - Setup bluetooth connection
- [imu_sensor](./Examples/Sensors/imu_sensor/)
    - Reading and displaying inertial measurement unit (IMU) data
- [imu_wake_display](./Examples/Sensors/imu_wake_display/)
    - Sleep functionality and wakeup on IMU motion detection
- [ppg_sensor](./Examples/Sensors/ppg_sensor/)
    - Reading and displaying Photoplethysmogram (ppg) sensor data (heart rate etc.)
- [temp_sensor](./Examples/Sensors/temp_sensor/)
    - Reading and displaying temperature sensor data
### user_study
- [user_study](./examples/user_study/study_controller/)
    - Code used to run the user study in the paper
    - Switching between the visualizations is possible by sending data via the BLE connection to the ring.
## src
- [SSD1320_OLED](./src/SSD1320_OLED.h)
    - OLED code modified to enable grayscale
- [VisRing](./src/VisRing.h)
    - Base grayscale visualization functions based on modified SSD1320_OLED code
- [VisRingUtility](./src/VisRingUtility.h)
    - Helper functions for the VisRing library managing sensor reading, bluetooth connection, and interaction functions.
