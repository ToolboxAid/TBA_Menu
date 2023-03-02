

#include "Arduino.h"

#include <FS.h>
#include <SD.h>
#include "SPIFFS.h"

#include "GlobalConst.h"

#include "Skin.h"

#include "Menu.h"

#include "pages/PageOK.h"
#include "pages/PageMain.h"
#include "pages/PageFileSelectControlWithHeader.h"
#include "pages/PageFileSelectControlWithOutHeader.h"

#include "TBA_SupportFunction.h"
TBA_SupportFunction supportFunction = TBA_SupportFunction();

void setup()
{ // Use serial port
  Serial.begin(115200);
  while (!Serial && (millis() <= 10000))
  { /* looping */
  };

  Serial.println();
  supportFunction.memoryInfo();

  Serial.println("Booting...");
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");
  Serial.println("ToolboxAid.com");
  Serial.println("Menu - File System");
  Serial.print("Tag: ");
  Serial.println(TAG);
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");

  /* Using TBA default skin */
  Skin *skin = new Skin(); 

  Menu::getInstance()->Initialize(skin, "ILI9341", PageMain::NAME);

  // Create the Main page
  Menu::getInstance()->addPage(new PageMain());

  // Create the File page w/ header
  Menu::getInstance()->addPage(new PageFileSelectControlWithHeader(skin));

  // // Create the File page w/o header
  Menu::getInstance()->addPage(new PageFileSelectControlWithOutHeader(skin));

  // Create the OK page
  Menu::getInstance()->addPage(new PageOK(skin));

  // Show memory differences
  supportFunction.memoryInfo();
}

void loop()
{
  Menu::getInstance()->checkMenuActions();
  delay(1);
  yield(); // Allow WDT to reset
}
