# Arduino ESP32 Library for ILI9341

![This is an image](./assets/ToolboxAid.png)

## Other than ILI9431, the library should also supports (not tested)

|           |             |             |             |                |
| --------- | ----------- | ----------- | ----------- | -------------- |
| ILI9341_2 | ST7735      | ILI9163     | S6D02A1     | RPI_ILI9486    |
| S6D02A1   | RPI_ILI9486 | HX8357D     | ILI9481     | ILI9488        |
| ILI9488   | ST7789      | ST7789_2    | R61581      | RM68140        |
| ST7796    | SSD1351     | SSD1963_480 | SSD1963_800 | SSD1963_800ALT |
| ILI9225   | GC9A01      |

## Libraries required (tag links)

- [TFT_eSPI v2.4.79](https://github.com/Bodmer/TFT_eSPI/releases/tag/v2.4.79)
- [TBA_LinkListPlus v1.0.0](https://github.com/ToolboxAid/TBA_LinkListPlus/releases/tag/v1.0.0)
- [TBA_SupportFunction v1.0.0](https://github.com/ToolboxAid/TBA_SupportFunction/releases/tag/v1.0.0)
- [TBA_Macros v1.0.2](https://github.com/ToolboxAid/TBA_Macros/releases/tag/v1.0.2)
- [TBA_FileSystem v1.0.0](https://github.com/ToolboxAid/TBA_FileSystem/releases/tag/v1.0.0)

## Configuration

In settings.json add "C_Cpp.intelliSenseEngine" to "Tag Parser", don't forget the comma

``` C++
 "C_Cpp.intelliSenseEngine": "Tag Parser"
```

Modify C:/Users/{user}/Documents/Arduino/libraries/TFT_eSPI/User_Setup_Select.h

``` Config
Line ::: Action     ::: Config code                                     ::: Description
  30 ::: comment    ::: // #include <User_Setup.h>                      ::: Default setup is root library folder
  79 ::: un-comment ::: #include <User_Setups/Setup42_ILI9341_ESP32.h>  ::: Setup file for ESP32 and SPI ILI9341 240x320
  80 ::: add        ::: #define TOUCH_CS 21                             ::: Chip select pin (T_CS) of touch screen
```

## Warning

Some displays do NOT have a 5V to 3.3V regulator fitted, so in that case you must ONLY connect 3V3 (3.3 Volts) to the display.
Check the display vendors web page and look for the power voltage needs.
A display that accepts only 3.3V may be destroyed when connecting 5V. Most survive for a while due to voltage drops but operation will be unreliable and the display may overheat.

## Wiring

### VSPI (the pins I am using)

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

### SPIClass spi = SPIClass(VSPI)

``` C++
spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
```
