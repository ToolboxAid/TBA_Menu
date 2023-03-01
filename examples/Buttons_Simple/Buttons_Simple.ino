/*
Required Packages:
https://github.com/janelia-arduino/Vector
*/

/*
In settings.json add "C_Cpp.intelliSenseEngine" to "Tag Parser", don't forget the comma
 "C_Cpp.intelliSenseEngine": "Tag Parser"

Modify => C:\Users\<user>\Documents\Arduino\libraries\TFT_eSPI\User_Setup_Select.h
Line ::: Action     ::: Info
  30 ::: comment    ::: // #include <User_Setup.h>           // Default setup is root library folder
  79 ::: un-comment ::: #include <User_Setups/Setup42_ILI9341_ESP32.h>           // Setup file for ESP32 and SPI ILI9341 240x320
  80 ::: add        ::: #define TOUCH_CS 21     // Chip select pin (T_CS) of touch screen

//VSPI (the pins I am using.)
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST   4  // Reset pin (could connect to RST pin)
//#define TFT_RST  -1// Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
#define TOUCH_CS 21     // Chip select pin (T_CS) of touch screen

//HSPI (Not Tested)
#define SD_SCK  14
#define SD_MISO  27
#define SD_MOSI  13
#define SD_CS  15

SPIClass spi = SPIClass(VSPI);
spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
*/

#include "Arduino.h"

#include <FS.h>
#include <SD.h>
#include "SPIFFS.h"

#include "GlobalConst.h"

#include "Skin.h"

#include "Menu.h"

#include "ElementPage.h"

#include "pages/PageMain.h"

void setup()
{ // Use serial port
  Serial.begin(115200);
  while (!Serial && (millis() <= 10000))
  {
  };

  Serial.println("\n\nBooting...");
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");
  Serial.println("ToolboxAid.com");
  Serial.println("Menu Test - Simple");
  Serial.print  ("Tag: ");
  Serial.println(TAG);
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");

  Skin *skin = new Skin(); /* Using TBA default skin */

 /*override TBA default skin
  skin = new Skin( name,  rotate,
         screenWidth,  screenHeight,
         headerHeight,  headerFontTextSize,  headerTextColor,  headerBackGroundColor,  headerIconImage,
         buttonTextColor,  buttonColor,  buttonShortColor,  buttonLongColor,  buttonBorderColor
         buttonMargin,  buttonBorderWidth,  buttonPadding,  buttonRadius,
         textFontSize,  textColor,  textBackgroundColor); */

  Menu::getInstance()->Initialize(skin, "ILI9341");

  ElementPage *page;

  // Create the page main
  PageMain *pageMain = new PageMain();
  Menu::getInstance()->addPage(pageMain);
}

void loop()
{
  Menu::getInstance()->checkMenuActions();
  delay(1);
  yield(); // Allow WDT to reset
}
