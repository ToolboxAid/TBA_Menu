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

    // Add label
    Point *point = new Point(10, 60);
    ElementLabel *label = new ElementLabel("The TBA intro page has", point);
    page->addLabel(label);

    point = new Point(10, 90);
    label = new ElementLabel("a hidden button on it.", point);
    page->addLabel(label);

    point = new Point(10, 140);
    label = new ElementLabel("Did you touch the screen?", point);
    page->addLabel(label);

    Dimensions *dimensions = new Dimensions(80, 190, 160, 30);
    ElementButton *button = new ElementButton("Back to TBA",
                                              /* X, Y, W, H             */ dimensions,
                                              /* short press            */ NULL,
                                              /* new short current page */ "Toolbox Aid",
                                              /* long  press            */ NULL,
                                              /* new long current page  */ NULL,
                                              /* icon                   */ NULL,
                                              /* value                  */ NULL,
                                              /* is Hidden              */ false);
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
