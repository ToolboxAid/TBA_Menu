/*
  PageMain.h - Library for PageMain code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageMain_h
#define PageMain_h

#include "Arduino.h"

#include "GlobalConst.h"

#include <sstream>
#include <string>

#include "Dimensions.h"

#include "ElementLabel.h"
#include "ElementButton.h"

// #include "PageBase.h"

class PageMain //: PageBase
{
private:
protected:
public:
  //  PageMain() : PageBase(){}

  static ElementPage *create(uint8_t across, uint8_t down, Skin *skin)
  {
    // Create Page
    const char *MAIN = "Main";
    ElementPage *page = new ElementPage(MAIN,
                                        /* static func to exec */ NULL,
                                        /* ms refrest seconds   */ 1,
                                        /* display Header      */ true,
                                        /* clear screen        */ true);

    // ElementPage::ElementPage(
    //   const char * name,
    //   void(* pRefreshFunction)(void),
    //   uint16_t refrestTimer,
    //   boolean displayHeader,
    //   boolean clearScreen,
    //   uint8_t backPageDelay,
    //   const char * backPage)

    // Add label
    Point *point = new Point(35, 40);
    ElementLabel *label = new ElementLabel("Show me a Button Maps", point);
    page->addLabel(label);

    Dimensions *dimensions = NULL;
    ElementButton *button = NULL;

    // 3x3 - createMapPage(3,3);
    dimensions = skin->getMapDimensions(across, down, 1, down - 2);
    button = new ElementButton("3x3",
                               /* X, Y, W, H             */ dimensions,
                               /* short press            */ NULL,
                               /* new short current page */ "3x3",
                               /* long  press            */ NULL,
                               /* new long current page  */ "3x3");
    page->addButton(button);

    // 4x4 - createMapPage(4,4);
    dimensions = skin->getMapDimensions(across, down, 2, down - 1);
    button = new ElementButton("4x4",
                               /* X, Y, W, H             */ dimensions,
                               /* short press            */ NULL,
                               /* new short current page */ "4x4",
                               /* long  press            */ NULL,
                               /* new long current page  */ "4x4");
    page->addButton(button);

    // 4x8 - createMapPage(4,8);
    dimensions = skin->getMapDimensions(across, down, 3, down);
    button = new ElementButton("4x8",
                               /* X, Y, W, H             */ dimensions,
                               /* short press            */ NULL,
                               /* new short current page */ "4x8",
                               /* long  press            */ NULL,
                               /* new long current page  */ "4x8");
    page->addButton(button);

    return page;
  }

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
