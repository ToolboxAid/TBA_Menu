/*
  PageTBA.h - Library for PageTBA code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageTBA_h
#define PageTBA_h

#include "Arduino.h"

#include "GlobalConst.h"
#include "Skin.h"

#include <sstream>
#include <string>

class PageTBA
{
private:
protected:
public:

    static ElementPage *create(const char * frontPage, uint16_t screenWidth)
    {
        // Create Page
        ElementPage *pageTBA = new ElementPage("Toolbox Aid",
        /* static func to exec  */ &PageTBA::pageTBARefresh,
        /* ms refrest seconds   */ 1,
        /* display Header       */ true,
        /* clear screen         */ true,
        /* backPageDelay seconds*/ 5,
        /* backPage             */ frontPage);


        uint16_t 
        textOffset = screenWidth / 2;
        textOffset -= (18 * 6 * 2) / 2;

      Point *point1 = new Point(textOffset, 80);
      ElementLabel *label1 = new ElementLabel("TBA Menu for ESP32", point1);
        pageTBA->addLabel(label1);
      
      Point *point2 = new Point(textOffset, 145);
      ElementLabel *label2 = new ElementLabel("www.toolboxaid.com",point2);
        pageTBA->addLabel(label2);

        textOffset = screenWidth / 2;
        textOffset -= (7 * 6 * 2) / 2;        
      Point *point3 = new Point(textOffset, 210);
      ElementLabel *label3 = new ElementLabel(TAG, point3);
        pageTBA->addLabel(label3);

        ElementButton *button = new ElementButton("Hidden Btn",
        /* X, Y, W, H             */ Display::getInstance()->getScreenDimensions(),
        /* short press            */ &PageTBA::tbaHiddenButtonShort,
        /* new short current page */ frontPage,
        /* long  press            */ &PageTBA::tbaHiddenButtonLong,
        /* new long current page  */ frontPage,
        /* icon                   */ NULL,
        /* value                  */ NULL,
        /* is Hidden              */ true);
        pageTBA->addButton(button);

        /* Don't do this, errors on display. (No NEW used)
        delete dimensions;
        delete button;
        delete pageTBA;
        */
        return pageTBA;
    }

    static void pageTBARefresh(/* void */)
    { // Do some work on a specific variables page
      // Serial.println("pageTBARefresh");
    }
    static void tbaHiddenButtonShort(const char * value)
    { // Do some work on a specific variables page
      // Serial.println("tbaHiddenButtonShort");
    }
    static void tbaHiddenButtonLong(const char * value)
    { // Do some work on a specific variables page
      // Serial.println("tbaHiddenButtonLong");
    }
};

#endif
