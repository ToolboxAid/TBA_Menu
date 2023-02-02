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

//#include "PageBase.h"

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
    Point *point = new Point(10, 60);
    ElementLabel *label = new ElementLabel("Hello world", point)    ;
    page->addLabel(label);

    // 2x2 - createMapPage(3,3);
    Dimensions *dimensions = new Dimensions(10, 90, 75, 25);
    ElementButton *button = new ElementButton("2x2",
    /* X, Y, W, H             */ dimensions,
    /* short press            */ NULL, //&StaticPage::pageMain,
    /* new short current page */ "2x2",
    /* long  press            */ NULL,
    /* new long current page  */ "2x2");
    page->addButton(button);

    // 3x3 - createMapPage(3,3);
    dimensions = new Dimensions(10, 120, 75, 25);
    button = new ElementButton("3x3",
    /* X, Y, W, H             */ dimensions,
    /* short press            */ NULL, //&StaticPage::pageMain,
    /* new short current page */ "3x3",
    /* long  press            */ NULL,
    /* new long current page  */ "3x3");
    page->addButton(button);

    // 4x4 - createMapPage(4,4);
    dimensions = new Dimensions(10, 150, 75, 25);
    button = new ElementButton("4x4",
    /* X, Y, W, H             */ dimensions,
    /* short press            */ NULL, //&StaticPage::pageMain,
    /* new short current page */ "4x4",
    /* long  press            */ NULL,
    /* new long current page  */ "4x4");
    page->addButton(button);

    // 4x8 - createMapPage(4,8);
    dimensions = new Dimensions(10, 180, 75, 25);
    button = new ElementButton("4x8",
    /* X, Y, W, H             */ dimensions,
    /* short press            */ NULL, //&StaticPage::pageMain,
    /* new short current page */ "4x8",
    /* long  press            */ NULL,
    /* new long current page  */ "4x8");
    page->addButton(button);

    dimensions = new Dimensions(10, 210, 75, 25);
    button = new ElementButton("Vars",
    /* X, Y, W, H             */ dimensions,
    /* short press            */ NULL, //&StaticPage::pageMain,
    /* new short current page */ "Vars Example",
    /* long  press            */ NULL,
    /* new long current page  */ "Vars Example");
    page->addButton(button);

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
