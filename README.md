# Adafruit ILI9341 Arduino Library [![Build Status](https://github.com/adafruit/Adafruit_ILI9341/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_ILI9341/actions)[![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](http://adafruit.github.io/Adafruit_ILI9341/html/index.html)

This is a library for the Adafruit ILI9341 display products

This library works with the Adafruit 2.8" Touch Shield V2 (SPI)
  * http://www.adafruit.com/products/1651

Adafruit 2.4" TFT LCD with Touchscreen Breakout w/MicroSD Socket - ILI9341
  * https://www.adafruit.com/product/2478

2.8" TFT LCD with Touchscreen Breakout Board w/MicroSD Socket - ILI9341
  * https://www.adafruit.com/product/1770

2.2" 18-bit color TFT LCD display with microSD card breakout - ILI9340
  * https://www.adafruit.com/product/1480

TFT FeatherWing - 2.4" 320x240 Touchscreen For All Feathers 
  * https://www.adafruit.com/product/3315

Check out the links above for our tutorials and wiring diagrams.
These displays use SPI to communicate, 4 or 5 pins are required
to interface (RST is optional).

**BMP image-loading examples have been moved to the Adafruit_ImageReader library:**
https://github.com/adafruit/Adafruit_ImageReader

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
MIT license, all text above must be included in any redistribution

To download. click the DOWNLOADS button in the top right corner, rename the uncompressed folder Adafruit_ILI9341. Check that the Adafruit_ILI9341 folder contains Adafruit_ILI9341.cpp and Adafruit_ILI9341.

Place the Adafruit_ILI9341 library folder your arduinosketchfolder/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE

Also requires the [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library) library for Arduino.

## Wiring:

In settings.json add "C_Cpp.intelliSenseEngine" to "Tag Parser", don't forget the comma
```
 "C_Cpp.intelliSenseEngine": "Tag Parser"
```

Modify => C:\Users\<user>\Documents\Arduino\libraries\TFT_eSPI\User_Setup_Select.h
Line ::: Action     ::: Info
```
  30 ::: comment    ::: // #include <User_Setup.h>                      // Default setup is root library folder
  79 ::: un-comment ::: #include <User_Setups/Setup42_ILI9341_ESP32.h>  // Setup file for ESP32 and SPI ILI9341 240x320
  80 ::: add        ::: #define TOUCH_CS 21                             // Chip select pin (T_CS) of touch screen
```

### VSPI (the pins I am using.)
* #define TFT_MISO 19
* #define TFT_MOSI 23
* #define TFT_SCLK 18
* #define TFT_CS   15  // Chip select control pin
* #define TFT_DC    2  // Data Command control pin
* #define TFT_RST   4  // Reset pin (could connect to RST pin)
* [#444444] //#define TFT_RST  -1// Set TFT_RST to -1 if display RESET is connected to ESP32 board RST [#FFFFFF]
* #define TOUCH_CS 21     // Chip select pin (T_CS) of touch screen

### HSPI (Not Tested)
* #define SD_SCK  14
* #define SD_MISO  27
* #define SD_MOSI  13
* #define SD_CS  15

### SPIClass spi = SPIClass(VSPI);
spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
