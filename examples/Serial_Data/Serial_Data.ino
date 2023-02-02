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

#include "pages/PageData.h"
#include "pages/PageOK.h"



// #define DEBUG

void setup()
{
  // Use serial port

  Serial.begin(115200);
  while (!Serial && (millis() <= 10000))
  {
  };

  Serial.println("Booting...");
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");
  Serial.println("ToolboxAid.com");
  Serial.println("Menu Tester");
  Serial.print  ("Tag: ");
  Serial.println(TAG);
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");

  u_int16_t TBA_ORANGE = Skin::rgb888torgb565(0xED9700);

  u_int16_t TBA_PURPLE = Skin::rgb888torgb565(0x3600AF); // 34%
  u_int16_t TBA_SHORT = Skin::rgb888torgb565(0x4600F0);  // 47%
  u_int16_t TBA_LONG = Skin::rgb888torgb565(0x7433FF);   // 60%

  u_int16_t TBA_GRAY = Skin::rgb888torgb565(0xBBBBBB);

  //skin = new Skin(); /* Using TBA default skin */
  Skin *skin = new Skin("TBA", Skin::ROTATE::SD_UP,
                  240, 320,
                  50-5, 3, TBA_ORANGE, TBA_PURPLE, NULL,
                  TBA_ORANGE, TBA_PURPLE, TBA_SHORT, TBA_LONG, TBA_GRAY,
                  5, 2, 3, 4,
                  2, TBA_PURPLE, TBA_ORANGE);

  Menu::getInstance()->init(*skin, "PageData");

  ElementPage *page;

  // Create the variables page
  page = PageData::create(skin);
  Menu::getInstance()->addPage(*page);

  // Create the OK page
  page = PageOK::create(skin);
  Menu::getInstance()->addPage(*page);
}

char serialBuffer[128];
void serialEvent1() {
  for (uint8_t x = 0; x<128;x++)
    serialBuffer[x]='\0';

  while (Serial.available()) {
    size_t size = Serial.readBytes(serialBuffer,128);
    Serial.print(size);
    Serial.print (" ");
    Serial.println(serialBuffer);
  }
}

const char * inputString;
bool serialEvent()
{
  if (Serial.available())
  {
    // get the new byte :
    char inChar = (char)Serial.read();

    //---------------------------------------------------
    // if the incoming character is a newline, return a flag
    // so the main loop can do something about it
    if (inChar == '\n')
//    if (inChar == '*')
    {
      inputString += '\0';
      return true;
    }

    //---------------------------------------------------
    // add it to the inputString:
    //if (!(inChar == '\r' || inChar == '\n'))
    else
    {
      inputString += inChar;
    }

    // if (inChar == '\n')
    // {
    //   Serial.println("NL");
    //   Serial.flush();
    // }
    // if (inChar == '\r')
    // {
    //   Serial.println("CR");
    //   Serial.flush();
    // }


  }
  return false;
}


void loop()
{
  Menu::getInstance()->checkMenuActions();
  if (serialEvent())
  { // Set inputData for display
    if (!Menu::getInstance()->setPageInput("inputData", inputString))
    {
      Serial.print("inputString update failed: ");
      Serial.println(inputString);      
    }

     // Send inputData to PC
    Serial.println(inputString);
    Serial.flush();

    // clear inputString
    inputString = NULL;;
  }
  delay(1);
  yield(); // Allow WDT to reset
}