/*
  PageIcons.h - Library for PageIcons code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageIcons_h
#define PageIcons_h

#include "Arduino.h"

#include <sstream>
#include <string>

class PageIcons : public ControlPage
{
private:
  inline static const char *NAME = "Icons";
  inline static const char *MAIN = "Main";
  inline static const char *PRINTER = "/3D_printer_80x80.png";
  inline static const char *XYZ = "/XYZ_80x80.png";
  inline static const char *INFO = "/info_80x80.png";
  inline static const char *CONF = "/config_80x80.png";

protected:
public:
  PageIcons(uint8_t across, uint8_t down, Skin *skin) : ControlPage(PageIcons::NAME, 1, false, true, 0, NULL)
  {
    Dimensions *dimensions;
    ControlButton *button;

    char *buffer = new char[4]{' ', ' ', ' ', '\0'};
    snprintf_P(buffer, sizeof(buffer), PSTR("%ix%i"), across, down);

    const char *icon = NULL;

    for (int x = 1; x <= across; x++)
    {
      for (int y = 1; y <= down; y++)
      {
        if (x == 1 && y == 1)
          icon = PRINTER;
        if (x == 1 && y == 2)
          icon = XYZ;
        if (x == 2 && y == 1)
          icon = INFO;
        if (x == 2 && y == 2)
          icon = CONF;

        buffer = new char[4]{' ', ' ', ' ', '\0'};
        snprintf_P(buffer, sizeof(buffer), PSTR("%ix%i"), x, y);

        this->addButton(new ControlButton((const char *)buffer, skin->getMapDimensions(across, down, x, y), PageIcons::MAIN, NULL, NULL, icon));
      }
    }
  }
};

void load()
{
  Serial.println("load(PageIcons)");
  Serial.flush();

  supportFunction.memoryInfo();
}
void refresh(/* void */)
{ // Do some work for page refresh
  Serial.println("refresh(PageIcons)");
  Serial.flush();
}
void exit()
{
  Serial.println("exit(PageIcons)");
  Serial.flush();
}

#endif
