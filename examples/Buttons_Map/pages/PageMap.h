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
  static ElementPage *create(uint8_t across, uint8_t down, Skin *skin)
  {
    Dimensions *dimensions;
    ElementButton *button;

    char *buffer = new char[4]{' ', ' ', ' ', '\0'};
    snprintf_P(buffer, sizeof(buffer), PSTR("%ix%i"), across, down);
    // const char *label = buffer;

    // ElementPage *page = new ElementPage(label,
    ElementPage *page = new ElementPage((const char *) buffer,
    
                                        /* static func to exec  */ NULL,
                                        /* ms refrest seconds   */ 1,
                                        /* display Header       */ false,
                                        /* clear screen         */ true,
                                        /* backPageDelay seconds*/ 10,
                                        /* backPage             */ "Main");


    bool addIcon = false;
    const char *icon = NULL;

    // add the buttons
    for (int x = 1; x <= across; x++)
    {
      for (int y = 1; y <= down; y++)
      {
        buffer = new char[4]{' ', ' ', ' ', '\0'};
        snprintf_P(buffer, sizeof(buffer), PSTR("%ix%i"), x, y);

        // Add button
        dimensions = skin->getMapDimensions(across, down, x, y);

        button = new ElementButton((const char *) buffer,
                                   /* X, Y, W, H             */ dimensions,
                                   /* short press static func*/ NULL,
                                   /* new short current page */ "Main",
                                   /* long press static func */ NULL,
                                   /* new long current page  */ "Main",
                                   /* icon                   */ icon,
                                   /* value                  */ NULL,
                                   /* is Hidden              */ false);

        page->addButton(button);
      }
    }

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
