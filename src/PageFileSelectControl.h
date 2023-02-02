/*
  PageFileSelectControl.h - Library for PageFileSelectControl code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageFileSelectControl_h
#define PageFileSelectControl_h

#include "Arduino.h"

#include "GlobalConst.h"
#include "Skin.h"

#include <sstream>
#include <string>

#include "ElementFile.h"
// #include "ElementButton.h"
#include "Dimensions.h"

class PageFileSelectControl
{
private:
protected:
public:
  static ElementPage *create(const char * name, const char * selectPage, const char * backPage, Skin *skin, const char * directory, bool showHeader = false)
  {
    // Create Page
    ElementPage *page = new ElementPage(name,
            /* static func to exec  */ &PageFileSelectControl::refresh,
            /* refrest seconds      */ 1,
            /* show Header          */ showHeader,
            /* clear screen         */ true,
            /* backPageDelay seconds*/ 254,// max 255
            /* backPage             */ backPage);

    ElementButton *button;
    Dimensions *dimensions;
    ElementFile *elementFile;
    ElementRectangle *rectangle;
    const char * icon;

    uint8_t size = 25;

    uint16_t top = 0;
    if (page->getDisplayHeader())
    {
      top = skin->getHeaderHeight();
    }

    // =============================================================================
    // === Scrole Bar ===
    // Add button: UP
    dimensions = new Dimensions(skin->getScreenWidth() - size - skin->buttonMargin, top + skin->buttonMargin, size, size);
    button = new ElementButton("U", dimensions, &PageFileSelectControl::pageUpButton, NULL, NULL, NULL, icon, "up", false);
    page->addButton(button);

    // Add button DW
    dimensions = new Dimensions(skin->getScreenWidth() - size - skin->buttonMargin, 240 - size - skin->buttonMargin, size, size);
    button = new ElementButton("D", dimensions, &PageFileSelectControl::pageDownButton, NULL, NULL, NULL, icon, "dn", false);
    page->addButton(button);

    // Add rectangle slider bar (between U & D)
    dimensions = new Dimensions(skin->getScreenWidth() - size - skin->buttonMargin, top + size, size, skin->getScreenHeight() - top - size - (skin->buttonMargin * 2));
    rectangle = new ElementRectangle("sidebar", dimensions);
    page->addRectangle(rectangle);

    // Add rectangle sidebar (position box)
    dimensions = new Dimensions(skin->getScreenWidth() - size, top + size, size - 10, size);
    rectangle = new ElementRectangle("slider", dimensions);
    page->addRectangle(rectangle);

    // =============================================================================
    // Add input directory name
    dimensions = new Dimensions(skin->buttonMargin, top + skin->buttonMargin, skin->getScreenWidth() - size - (skin->buttonMargin * 2) - (skin->buttonBorderWidth * 2), 22);
    page->addInput(new ElementInput("inputDirectory", "Directory name here", dimensions));
    // =============================================================================
    // Add rectangles back drop
    top = dimensions->getY() + dimensions->getH() + skin->buttonMargin;
    dimensions = new Dimensions(skin->buttonMargin, top, skin->getScreenWidth() - size - (skin->buttonMargin * 2) - (skin->buttonBorderWidth * 2), skin->getScreenHeight() - top - (skin->buttonMargin * 1));
    rectangle = new ElementRectangle("backdrop", dimensions);
    page->addRectangle(rectangle);

    return page;
  }

  static void refresh(/* void */)
  { // Do some work on a specific variables page

Serial.println("-------------------------------------------------------------------------------------");
Menu::getInstance()->getCurrentPage()->buttonListPlus->traverseForward();
Serial.println("-------------------------------------------------------------------------------------");
    Serial.print("Refresh: ");
    static uint16_t cnt = 0;
    Serial.print(cnt++);

    Dimensions *dimensions;
    ElementFile *elementFile;
    ElementRectangle *rectangle;

    // =============================================================================
    // Add a button per File // Just for show here

    rectangle = Menu::getInstance()->getPageRectangle("backdrop");
    if (rectangle)
    {
      uint16_t top = rectangle->getDimensions()->getH();
      uint8_t loopSize = top / 20;
      Serial.print(" loopSize: ");
      Serial.print(loopSize);
      Menu::getInstance()->getCurrentPage()->clearFiles();
      for (int x = 0; x < loopSize; x++)
      { //                             x,                                                              y,                                              w,   h
        dimensions = new Dimensions(rectangle->getDimensions()->getX(), rectangle->getDimensions()->getY() + (x * 20), rectangle->getDimensions()->getW(), 20);
        elementFile = new ElementFile("Filename goes here", dimensions, &PageFileSelectControl::pageFileSelect, "PageOK", "full file path", NULL);

        Menu::getInstance()->getCurrentPage()->addButton(elementFile);
        // Menu::getInstance()->addFile(*elementFile);
      }
    }
    else
    {
      Serial.println("Rect not found: 'backdrop'");
    }
Serial.println();
  }

  static void pageUpButton(const char * value)
  { // Do some work on a specific variables page
    uint16_t top = 60;
    uint8_t size = 40;

    // if (page->getClearScreen());
    // {
    //   top = skin->getHeaderHeight();
    // }
    //    Display::getInstance()->getScreenTFT().fillRoundRect(320 - size, top, size, 180 - (size * 2), 5, Skin::rgb888torgb565(0xFFFF00));

    PageFileSelectControl::refresh();
  }

  static void pageDownButton(const char * value)
  { // Do some work on a specific variables page
    uint16_t top = 60;
    uint8_t size = 40;

    // if (page->getClearScreen());
    // {
    //   top = skin->getHeaderHeight();
    // }
    //    Display::getInstance()->getScreenTFT().fillRoundRect(320 - size, top, size, 200 - (size * 2), 5, Skin::rgb888torgb565(0x00FF00));

    PageFileSelectControl::refresh();
  }

  static void pageFileSelect(const char * value)
  { // Do some work on a specific variables page
    uint16_t top = 60;
    uint8_t size = 40;

    // if (page->getClearScreen());
    // {
    //   top = skin->getHeaderHeight();
    // }
    Display::getInstance()->getScreenTFT().fillRoundRect(320 - size, top, size, 220 - (size * 2), 5, Skin::rgb888torgb565(0x00FFFF));
  }
};

#endif
