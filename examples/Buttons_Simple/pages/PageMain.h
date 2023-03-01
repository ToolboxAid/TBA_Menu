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

#include "TBA_SupportFunction.h"
TBA_SupportFunction supportFunction = TBA_SupportFunction();

class PageMain : public ElementPage
{
private:
  inline static const char *NAME = "Main";
  inline static const char *LABEL1 = "The TBA intro page has";
  inline static const char *LABEL2 = "a hidden button on it.";
  inline static const char *LABEL3 = "Did you touch the screen?";
  inline static const char *BUTTON = "Back to TBA";
  inline static const char *SHORT = "Toolbox Aid";

protected:
public:
  PageMain() : ElementPage(PageMain::NAME, 3, true, true, 0, NULL)
  {
    this->addLabel(new ElementLabel(PageMain::LABEL1, new Point(10, 60)));
    this->addLabel(new ElementLabel(PageMain::LABEL2, new Point(10, 90)));
    this->addLabel(new ElementLabel(PageMain::LABEL3, new Point(10, 140)));
    this->addButton(new ElementButton(PageMain::BUTTON, new Dimensions(80, 190, 160, 30), PageMain::SHORT));
  }

  void load()
  {
    supportFunction.memoryInfo();
  }
};

#endif
