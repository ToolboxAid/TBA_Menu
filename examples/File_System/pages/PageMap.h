/*
  PageMap.h - Library for PageMap code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageMap_h
#define PageMap_h

#include "Arduino.h"

#include "GlobalConst.h"

// ConvertString
// #include <iostream>
#include <sstream>
#include <string>

class PageMap
{
private:
protected:
public:
static ElementPage *createPage(uint8_t across, uint8_t down)
{
  Dimensions *dimensions;
  ElementButton *button;
  char buffer[10];

  snprintf_P(buffer, sizeof(buffer), PSTR("%ix%i"), across, down);
  const char * label = buffer;
  ElementPage *page = new ElementPage(label,
  /* static func to exec  */ NULL,
  /* ms refrest seconds   */ 1,
  /* display Header       */ false,
  /* clear screen         */ true,
  /* backPageDelay seconds*/ 10,
  /* backPage             */ "Main");

// If 2x2 grid, add icon to button
bool addIcon = false;
const char * icon;

bool showDiffIcon = false;
if (label.compare("2x2") == 0)
{
  addIcon = true;
  //icon.append("/gcode/XYZ.png");
  //icon.append("/menu/3D_printer_100x100.png");
  showDiffIcon = true;  
    // icon.append("/3D_printer_80x80.png");
 }

  // add the buttons
  for (int x = 1; x <= across; x++)
  {
    for (int y = 1; y <= down; y++)
    {
      if (showDiffIcon)
      {
        icon.erase(0);
        if (x == 1 && y == 1)
            icon.append("/3D_printer_80x80.png");
        if (x == 1 && y == 2)
            icon.append("/XYZ_80x80.png");
        if (x == 2 && y == 1)
            icon.append("/info_80x80.png");
        if (x == 2 && y == 2)
            icon.append("/config_80x80.png");
      }
      snprintf_P(buffer, sizeof(buffer), PSTR("%i - %i"), x, y);
      label = buffer;

      // Add button
      dimensions = skin->getMapDimensions(across, down, x, y);
 
      button = new ElementButton(label,
      /* X, Y, W, H             */ dimensions,
      /* short press static func*/ NULL,
      /* new short current page */ "Main",
      /* long press static func */ NULL,
      /* new long current page  */ "Main",
      /* icon                   */ icon,
      /* value                  */ NULL,
      /* is Hidden              */ false);

      // if (addIcon)
      //   button->debugSerial("PageMap");  

      page->addButton(*button);
    }
  }
  // Add Page to menu
  Menu::getInstance()->addPage(*page);

  /* Don't do this, errors on display.
  delete dimensions;
  delete button;
  */

    return page;
  }

  /* Formating options...
  Format	Type      bytes
  ======  ========  =====
  %c   	char      1/char
  %i  	int       4
  %u  	unsigned  4
  %f  	float     4
          double    8
  %s  	string    1/char + 1 for terminater
  */
  static void pageRefresh()
  { // Do some work on a specific variables page
    // Serial.println("pageRefresh");
  }
  static void hiddenButtonShort()
  { // Do some work on a specific variables page
    // Serial.println("hiddenButtonShort");
  }
  static void hiddenButtonLong()
  { // Do some work on a specific variables page
    // Serial.println("hiddenButtonLong");
  }
};

#endif
