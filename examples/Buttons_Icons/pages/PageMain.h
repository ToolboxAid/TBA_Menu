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

#include "TBA_FileSystem.h"
TBA_FileSystem fileSystem = TBA_FileSystem();

#include "TBA_SupportFunction.h"
TBA_SupportFunction supportFunction = TBA_SupportFunction();

class PageMain : public ElementPage
{
private:
  inline static const char *LABEL1 = "Show me Buttons w/ Icons!";
  inline static const char *BUTTON = "Show Icon Buttons";
  inline static const char *ICONS = "Icons";

protected:

public:
  inline static const char *NAME = "Main";

  PageMain() : ElementPage(PageMain::NAME, 0, true, true, 0, NULL)
  {
    this->addLabel(new ElementLabel(PageMain::LABEL1, new Point(10, 100)));
    this->addButton(new ElementButton(PageMain::BUTTON, new Dimensions(50, 190, 225, 25), "Icons"));
  }

void load()
{
  Serial.println("load(PageMain)");
  supportFunction.memoryInfo();
}
void refresh(/* void */)
{ // Do some work for page refresh
  Serial.println("refresh(PageMain)");
}
void exit()
{
  Serial.println("exit(PageMain)");
}
};

#endif
