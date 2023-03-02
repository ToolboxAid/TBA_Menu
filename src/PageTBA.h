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

#include "Dimensions.h"

#include <sstream>
#include <string>

class PageTBA : public ControlPage
{
private:
  inline static const char *NAME = "Toolbox Aid";
  inline static const char *LABEL1 = "TBA Menu for ESP32";
  inline static const char *LABEL2 = "www.toolboxaid.com";
  inline static const char *BUTTON = "Hidden Btn";

protected:
public:
  PageTBA(const char *frontPage, Skin *skin) : ControlPage(PageTBA::NAME, 0, true, true, 5, frontPage, "/TBA_40x25.png")
  {
    uint16_t textOffset = skin->getScreenWidth() / 2;
    textOffset -= (18 * 6 * 2) / 2;

    this->addLabel(new ControlLabel(PageTBA::LABEL1, new Point(textOffset, 80)));
    this->addLabel(new ControlLabel(PageTBA::LABEL2, new Point(textOffset, 145)));

    textOffset = skin->getScreenWidth() / 2;
    textOffset -= (7 * 6 * 2) / 2;
    this->addLabel(new ControlLabel(TAG, new Point(textOffset, 210)));

    this->addButton(new ControlButton(PageTBA::BUTTON, new Dimensions(0, 0, skin->getScreenWidth(), skin->getScreenHeight()), frontPage, frontPage, NULL, NULL, true));
  }
};

#endif
