/*
  PageMap.h - Library for PageMap code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageMap_h
#define PageMap_h

#include "Arduino.h"

#include "ControlButton.h"


class PageMap : public ControlPage
{
private:
  inline static const char *MAIN = "Main";

protected:
public:
  PageMap(const char *name, uint8_t across, uint8_t down, Skin *skin__) : ControlPage(name, 0, false, true, 0, NULL, NULL)
  {
    char *buffer = new char[4]{' ', ' ', ' ', '\0'};
    snprintf_P(buffer, sizeof(buffer), PSTR("%ix%i"), across, down);
    // const char *label = buffer;

    boolean addIcon = false;
    const char *icon = NULL;

    // add the buttons
    for (int x = 1; x <= across; x++)
    {
      for (int y = 1; y <= down; y++)
      {
        buffer = new char[4]{' ', ' ', ' ', '\0'};
        snprintf_P(buffer, sizeof(buffer), PSTR("%ix%i"), x, y);

        // Add button
        ControlButton *button = new ControlButton((const char *)buffer, skin->getMapDimensions(across, down, x, y), PageMap::MAIN);
        this->addButton(button);
      }
    }
  }
  void load()
  {
    supportFunction.memoryInfo();
  }
};

#endif
