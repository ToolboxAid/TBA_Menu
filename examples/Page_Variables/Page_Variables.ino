

#include "Arduino.h"

#include <FS.h>
#include <SD.h>
#include "SPIFFS.h"

#include "GlobalConst.h"

#include "Skin.h"

#include "Menu.h"

#include "ElementPage.h"

#include "pages/PageMain.h"
#include "pages/PageVariable.h"

void setup()
{
 // Use serial port
  Serial.begin(115200);
  while (!Serial && (millis() <= 10000))
  {
  };

  Serial.println("\n\nBooting...");
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");
  Serial.println("ToolboxAid.com");
  Serial.println("Menu Test - Variables");
  Serial.print  ("Tag: ");
  Serial.println(TAG);
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");

  Skin *skin = new Skin(); /* Using TBA default skin */

  Menu::getInstance()->Initialize(skin, "ILI9341");

  // Create the menu page
  Menu::getInstance()->addPage(new PageMain());

  // Create the variables page
  Menu::getInstance()->addPage(new PageVariable());
}

void loop()
{
  Menu::getInstance()->checkMenuActions();
  delay(1);
  yield(); // Allow WDT to reset
}