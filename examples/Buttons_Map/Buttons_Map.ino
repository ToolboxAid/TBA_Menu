
#include "Arduino.h"

#include "GlobalConst.h"

#include "Skin.h"

#include "Menu.h"

#include "ControlPage.h"

#include "pages/PageMain.h"
#include "pages/PageMap.h"

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
  Serial.println("Menu Test - Button Map");
  Serial.print  ("Tag: ");
  Serial.println(TAG);
  Serial.println("- - - - - - - - - - - - - - - - - - - - -");

  /* Using TBA default skin */
  Menu::getInstance()->Initialize(new Skin(), "ILI9341", PageMain::NAME);

  // Create the menu page
  Menu::getInstance()->addPage(new PageMain(3,4));

  // Create the 3x3 map page
  Menu::getInstance()->addPage(new PageMap(PageMain::BUTTON3X3, 3, 3));

  // Create the 4x4 map page
  Menu::getInstance()->addPage(new PageMap(PageMain::BUTTON4X4, 4, 4));

  // Create the 4x8 map page
  Menu::getInstance()->addPage(new PageMap(PageMain::BUTTON4X8, 4, 8));
}

void loop()
{
  Menu::getInstance()->checkMenuActions();
  delay(1);
  yield(); // Allow WDT to reset
}