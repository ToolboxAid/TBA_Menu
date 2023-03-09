
#include "Arduino.h"

#include "GlobalConst.h"

#include "Skin.h"

#include "Menu.h"

#include "pages/PageMain.h"
#include "pages/PageIcons.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial && (millis() <= 10000))
  {
  };

  Serial.println("\n\nBooting...");
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");
  Serial.println("ToolboxAid.com");
  Serial.println("Menu Test - Buttons w/Icons");
  Serial.print("Tag: ");
  Serial.println(TAG);
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");

  /* Using TBA default skin */
  Menu::getInstance()->Initialize(NULL, "ILI9341", PageMain::NAME);

  // Create the main page
  Menu::getInstance()->addPage(new PageMain());

  // Create the 2x2 map page
  Menu::getInstance()->addPage(new PageIcons(2, 2));
}

void loop()
{
  Menu::getInstance()->checkMenuActions();
  delay(1);
  yield(); // Allow WDT to reset
}