
#include "Arduino.h"

#include <FS.h>
#include <SD.h>
#include "SPIFFS.h"

#include "GlobalConst.h"

#include "Skin.h"

#include "Menu.h"

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
         headerHeight,  headerFontTextSize,  headerTextColor,  headerBackGroundColor,
         buttonTextColor,  buttonColor,  buttonShortColor,  buttonLongColor,  buttonBorderColor
         buttonMargin,  buttonBorderWidth,  buttonPadding,  buttonRadius,
         textFontSize,  textColor,  textBackgroundColor); */

  Menu::getInstance()->Initialize(skin, "ILI9341");

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
