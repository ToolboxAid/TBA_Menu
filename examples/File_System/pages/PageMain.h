/*
  PageMain.h - Library for PageMain code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageMain_h
#define PageMain_h

#include "Arduino.h"

#include <sstream>
#include <string>


#include "Dimensions.h"
#include "Point.h"
#include "ElementButton.h"
#include "ElementLabel.h"

#include "TBA_SupportFunction.h"
extern TBA_SupportFunction supportFunction;

class PageMain : public ElementPage
{
private:
  inline static const char *LABEL1 = "Show me some files";
  inline static const char *LABEL2 = "File Select Examples";
  inline static const char *BUTTONW = "w/ Header";
  inline static const char *BUTTONWO = "w/o Header";

protected:
public:
  inline static const char *NAME = "Main";

  PageMain() : ElementPage(PageMain::NAME, 0, true, true, 0, NULL)
  {
    // Add label
    this->addLabel(new ElementLabel(LABEL1, new Point(10, 50)));
    this->addLabel(new ElementLabel(LABEL2, new Point(10, 90)));

    this->addButton(new ElementButton(BUTTONW, new Dimensions(15, 150, 290, 35), BUTTONW));
    this->addButton(new ElementButton(BUTTONWO, new Dimensions(15, 190, 290, 35), BUTTONWO));
  }

  void load()
  {
    supportFunction.memoryInfo();
  }
  void refresh()
  {
  }
  void exit()
  {
  }
};

#endif
