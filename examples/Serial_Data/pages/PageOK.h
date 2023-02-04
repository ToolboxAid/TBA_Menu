/*
  PageOK.h - Library for PageOK code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageOK_h
#define PageOK_h

#include "Arduino.h"

#include "GlobalConst.h"

#include <sstream>
#include <string>

#include "Skin.h"
#include "PageBase.h"

#include "ElementRectangle.h"


class PageOK
{
private:
protected:
public:

  static ElementPage *create(Skin *skin)
  {
    ElementPage *page = new ElementPage("PageOK",
              /* static func to exec  */ NULL,
              /* ms refrest seconds   */ 0,
              /* display Header       */ false,
              /* clear screen         */ false); 
              // ,
              // /* backPageDelay seconds*/ 0,
              // /* backPage             */ NULL);

    Dimensions *dimensions;
    ElementButton *button;
    ElementRectangle *rectangle;

    // button layout
    uint8_t across = 3;
    uint8_t down = 6;

    // =============================================================================
    // Add buttons: clear, Back, Send
    dimensions = skin->getMapDimensions(across, down, 2, 5);
    button = new ElementButton("OK", dimensions, NULL, "PageData", NULL, NULL);
    page->addButton(button);

    // Add label
    page->addLabel(new ElementLabel("Data Sent", new Point(65, 140), false));

    // Add rectangle DW
    uint8_t top = 40;
    uint8_t edg = 25;
    dimensions = new Dimensions(skin->buttonMargin + edg, top, skin->getScreenWidth() - (skin->buttonMargin * 2) - (edg * 2), skin->getScreenHeight() - top - (skin->buttonMargin * 2));
    rectangle = new ElementRectangle("backdrop", dimensions);
    page->addRectangle(rectangle);
    
    /* Don't do this, errors on display.
    delete dimensions;
    delete button;
    */
    return page;
  }
};

#endif
