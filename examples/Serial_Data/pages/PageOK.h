/*
  PageOK.h - Library for PageOK code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageOK_h
#define PageOK_h

#include "Arduino.h"

#include <sstream>
#include <string>

#include "Skin.h"
#include "Point.h"

#include "ControlButton.h"
#include "ControlLabel.h"
#include "ControlRectangle.h"

class PageOK : public ControlPage
{
private:
  inline static const char *NAME = "PageOK";

protected:
public:
  
  PageOK(Skin *skin) : ControlPage(PageOK::NAME, 0, true, true, 0, NULL)
  {
    Dimensions *dimensions;

    // button layout
    uint8_t across = 3;
    uint8_t down = 6;

    // =============================================================================
    // Add buttons: clear, Back, Send
    dimensions = skin->getMapDimensions(across, down, 2, 5);
    this->addButton(new ControlButton("OK", dimensions, PageData::NAME));

    // Add label
    this->addLabel(new ControlLabel("Data Sent", new Point(65, 140), false));

    // Add rectangle DW
    uint8_t top = 40;
    uint8_t edg = 25;
    dimensions = new Dimensions(skin->buttonMargin + edg, top, skin->getScreenWidth() - (skin->buttonMargin * 2) - (edg * 2), skin->getScreenHeight() - top - (skin->buttonMargin * 2));
    this->addRectangle(new ControlRectangle("backdrop", dimensions));
  }
};

#endif
