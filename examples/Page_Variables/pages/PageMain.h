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
#include "Dimensions.h"

#include "ElementButton.h"
#include "ElementLabel.h"

#include "TBA_SupportFunction.h"
TBA_SupportFunction supportFunction = TBA_SupportFunction();

class PageMain : public ElementPage
{
private:
  inline static const char *NAME = "Main";
  inline static const char *VALUE = "Go to Vars Page";
  inline static const char *VARS = "Vars Example";
  inline static const char *LABEL1 = "Show me Buttons w/ Icons!";
  inline static const char *BUTTON = "Show Icon Buttons";
  inline static const char *ICONS = "Icons";

protected:
public:
  PageMain() : ElementPage(PageMain::NAME, 0, true, true, 0, NULL)
  {
    // Add labels
    this->addLabel(new ElementLabel("The below button will", new Point(10, 60)));
    this->addLabel(new ElementLabel("show you the Vars Example", new Point(10, 80)));

    this->addButton(new ElementButton(PageMain::VALUE,new Dimensions(40, 130, 240, 30),PageMain::VARS));

    this->addLabel(new ElementLabel("FYI:", new Point(10, 190)));
    this->addLabel(new ElementLabel("Vars Page uses refresh()", new Point(10, 210)));
  }

  void load()
  {
    supportFunction.memoryInfo();
  }
};

#endif
