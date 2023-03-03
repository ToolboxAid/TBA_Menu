/*
  PageMain.h - Library for PageMain code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageMain_h
#define PageMain_h

#include "Arduino.h"

#include "Point.h"

#include "ControlLabel.h"
#include "ControlButton.h"

#include "TBA_SupportFunction.h"
TBA_SupportFunction supportFunction = TBA_SupportFunction();

class PageMain : public ControlPage
{
private:
  inline static const char *LABEL1 = "This page is a Button Map";
  inline static const char *LABEL2 = "Press a";
  inline static const char *LABEL3 = "Button to";
  inline static const char *LABEL4 = "see more Maps";

protected:
public:
  inline static const char *NAME = "Main";
  inline static const char *BUTTON3X3 = "3x3";
  inline static const char *BUTTON4X4 = "4x4";
  inline static const char *BUTTON4X8 = "4x8";

  PageMain(uint8_t across, uint8_t down) : ControlPage(PageMain::NAME, 0, true, true, 0, NULL, NULL)
  {
    // Add labels
    this->addLabel(new ControlLabel(PageMain::LABEL1, new Point(10, 40)));
    this->addLabel(new ControlLabel(PageMain::LABEL2, new Point(5, 180)));
    this->addLabel(new ControlLabel(PageMain::LABEL3, new Point(5, 200)));
    this->addLabel(new ControlLabel(PageMain::LABEL4, new Point(5, 220)));

    // 3x3 - createMapPage(3,3);
    this->addButton(new ControlButton(PageMain::BUTTON3X3, skin->getMapDimensions(across, down, 1, down - 2), PageMain::BUTTON3X3));

    // 4x4 - createMapPage(4,4);
    this->addButton(new ControlButton(PageMain::BUTTON4X4, skin->getMapDimensions(across, down, 2, down - 1), PageMain::BUTTON4X4));

    // 4x8 - createMapPage(4,8);
    this->addButton(new ControlButton(PageMain::BUTTON4X8, skin->getMapDimensions(across, down, 3, down - 0), PageMain::BUTTON4X8));
  }

  void load()
  {
    supportFunction.memoryInfo();
  }
};

#endif
