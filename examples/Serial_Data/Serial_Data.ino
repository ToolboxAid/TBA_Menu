
#include "Arduino.h"

#include <FS.h>
#include <SD.h>
#include "SPIFFS.h"

#include "GlobalConst.h"

#include "Skin.h"

#include "Menu.h"

#include "pages/PageData.h"
#include "pages/PageOK.h"

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
  Serial.println("Menu - Serial data");
  Serial.print("Tag: ");
  Serial.println(TAG);
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");

  u_int16_t TBA_ORANGE = Skin::rgb888torgb565(0x333333);

  u_int16_t TBA_PURPLE = Skin::rgb888torgb565(0xAAAAAA); // 34%
  u_int16_t TBA_SHORT = Skin::rgb888torgb565(0xCCCCCC);  // 47%
  u_int16_t TBA_LONG = Skin::rgb888torgb565(0xEEEEEE);   // 60%

  u_int16_t TBA_GRAY = Skin::rgb888torgb565(0x666666);

  /* NOT using TBA skin */
  Skin *skin = new Skin("NotTBA", Skin::ROTATE::SD_UP,
                        240, 320,
                        50 - 5, 3, TBA_ORANGE, TBA_PURPLE,
                        TBA_ORANGE, TBA_PURPLE, TBA_SHORT, TBA_LONG, TBA_GRAY,
                        5, 2, 3, 4,
                        2, TBA_PURPLE, TBA_ORANGE);

  Menu::getInstance()->Initialize(skin, "ILI9341", PageData::NAME);

  // Create the data page
  Menu::getInstance()->addPage(new PageData(skin));

  // Create the OK page
  Menu::getInstance()->addPage(new PageOK(skin));
}

uint16_t serialOffset = 0;
char serialBuffer[100];
void serialEvent()
{
  if (Serial.available())
  {
    while (Serial.available())
    {
      serialBuffer[serialOffset] = Serial.read();

      if (serialBuffer[serialOffset] == '\n')
      {
        serialBuffer[serialOffset] = '\0'; // Append a null terminator

        ControlInput *findInput = Menu::getInstance()->getCurrentPage()->getPageInput(PageData::inputName);
        if (findInput)
        {
          findInput->clear();
          for (int loop = 0; loop < serialOffset; loop++)
          {
            findInput->append(serialBuffer[loop]);
          }
        }
        serialOffset = 0;
      }
      else
      {
        serialOffset++;
      }
    }
  }
}

void loop()
{
  Menu::getInstance()->checkMenuActions();
  serialEvent();
  delay(1);
  yield(); // Allow WDT to reset
}
