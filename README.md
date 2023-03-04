# Adafruit ILI9341 Arduino Library [![Build Status](https://github.com/adafruit/Adafruit_ILI9341/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_ILI9341/actions)[![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](http://adafruit.github.io/Adafruit_ILI9341/html/index.html)

## This is a library for the Adafruit ILI9341 display products

### Other than ILI9431, the library also supports:
|           |             |             |             |               |
| --------- | ----------- | ----------- | ----------- | ------------  |
| ILI9341_2 | ST7735      | ILI9163     | S6D02A1     | RPI_ILI9486   |
| S6D02A1   | RPI_ILI9486 | HX8357D     | ILI9481     | ILI9488        |
| ILI9488   | ST7789      | ST7789_2    | R61581      | RM68140        |
| ST7796    | SSD1351     | SSD1963_480 | SSD1963_800 | SSD1963_800ALT |
| ILI9225   | GC9A01      |

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
```diff
  #define TFT_MISO 19
  #define TFT_MOSI 23
  #define TFT_SCLK 18
  #define TFT_CS   15  // Chip select control pin
  #define TFT_DC    2  // Data Command control pin
  #define TFT_RST   4  // Reset pin (could connect to RST pin)
- #define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
  #define TOUCH_CS 21  // Chip select pin (T_CS) of touch screen
```

### HSPI (Not Tested)
```diff
  #define SD_SCK   14
  #define SD_MISO  27
  #define SD_MOSI  13
  #define SD_CS    15
```

### SPIClass spi = SPIClass(VSPI);
```
spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
```
