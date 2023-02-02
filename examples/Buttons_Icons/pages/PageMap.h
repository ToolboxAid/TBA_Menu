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

#include <sstream>
#include <string>

class PageMap
{
private:
protected:
public:
static ElementPage *create(uint8_t across, uint8_t down, Skin  *skin)
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
//if (label.compare("2x2") == 0)
if (strcmp(label,"2x2") == 0)
{
  addIcon = true;
  //icon.append("/gcode/XYZ.png");
  //icon.append("/menu/3D_printer_100x100.png");
  showDiffIcon = true;  
    // icon.append("/3D_printer_80x80.png");
 }

icon = NULL;

  // add the buttons
  for (int x = 1; x <= across; x++)
  {
    for (int y = 1; y <= down; y++)
    {
      if (showDiffIcon)
      {
        if (x == 1 && y == 1)
            //icon.append("/3D_printer_80x80.png");
            icon = "/3D_printer_80x80.png";
        if (x == 1 && y == 2)
            //icon.append("/XYZ_80x80.png");
            icon = "/XYZ_80x80.png";
        if (x == 2 && y == 1)
            //icon.append("/info_80x80.png");
            icon = "/info_80x80.png";
        if (x == 2 && y == 2)
            //icon.append("/config_80x80.png");
            icon = "/config_80x80.png";
      }


int size;
char str[64];         // statically allocated
char *new_str = NULL; // for dynamically allocated

strcpy(str, "Hello");
size = strlen(str);   // returns 5

new_str = (char *)malloc(sizeof(char) * (size+1)); // need space for '\0'
if(new_str == NULL) {
    printf("Error: malloc failed!  exiting.\n");
    exit(1);
}
strcpy(new_str, str);
printf("%s %s\n", str, new_str);    // prints "Hello Hello"

// //--------------------
// char *c = (char *)malloc(sizeof(char) * (5));
// snprintf_P(c, 5, PSTR("%ix%i\0"), x, y);
// // for (uint8_t x = 0; x < strlen(c)+1; x++)
// //   c[x] = buffer[x];
// //--------------------
// Serial.println("*c");
// Serial.println(strlen(c));
// Serial.println(c);

      // Add button
      dimensions = skin->getMapDimensions(across, down, x, y);
 
      button = new ElementButton("2x2",
      /* X, Y, W, H             */ dimensions,
      /* short press static func*/ NULL,
      /* new short current page */ "Main",
      /* long press static func */ NULL,
      /* new long current page  */ "Main",
      /* icon                   */ icon,
      /* value                  */ NULL,
      /* is Hidden              */ false);
button->debugSerial("PageMap");  
      // if (addIcon)
      //   button->debugSerial("PageMap");  

      page->addButton(button);
    }
  }
  // Add Page to menu
  Menu::getInstance()->addPage(page);

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
