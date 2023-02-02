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

  static ElementPage *create()
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
    Point *point = new Point(10, 100);
    ElementLabel *label = new ElementLabel("Show me Buttons w/ Icons!", point);
    page->addLabel(label);

    Dimensions *dimensions = new Dimensions(50, 190, 225, 25);
    ElementButton *button = new ElementButton("Show Icon Buttons",
                                              /* X, Y, W, H             */ dimensions,
                                              /* short press            */ NULL,
                                              /* new short current page */ "2x2",
                                              /* long  press            */ NULL,
                                              /* new long current page  */ "2x2");
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
