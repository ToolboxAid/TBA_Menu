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

//#include "PageBase.h"

class PageMain //: PageBase
{
private:
protected:
public:

//  PageMain() : PageBase(){}

  static ElementPage *create(Skin *skin)
  {
    Dimensions *dimensions;
    ElementButton *button;
    //ElementFile *fileElement;

    // Create Page
    ElementPage *page = new ElementPage("Main",
    /* static func to exec */ NULL,
    /* ms refrest seconds   */ 1,
    /* display Header      */ true,
    /* clear screen        */ true);

    // Add label
    page->addLabel(*(new ElementLabel("Hello world", new Point(10, 50))));
    page->addLabel(*(new ElementLabel("File Select Example", new Point(10, 90))));

    dimensions = new Dimensions(20, 140, 280, 35);
    button = new ElementButton("File Select w/ header",
    /* X, Y, W, H             */ dimensions,
    /* short press            */ NULL, //&StaticPage::pageMain,
    /* new short current page */ "File Select w/ header",
    /* long  press            */ NULL,
    /* new long current page  */ NULL);
    page->addButton(*button);

    dimensions = new Dimensions(20, 180, 280, 35);
    button = new ElementButton("File Select w/o header",
    /* X, Y, W, H             */ dimensions,
    /* short press            */ NULL, //&StaticPage::pageMain,
    /* new short current page */ "File Select w/o header",
    /* long  press            */ NULL,
    /* new long current page  */ NULL);
    page->addButton(*button);


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
};

#endif
