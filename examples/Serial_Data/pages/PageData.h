/*
  PageData.h - Library for PageData code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageData_h
#define PageData_h

#include "Arduino.h"

#include "GlobalConst.h"

#include <sstream>
#include <string>

#include "Skin.h"
#include "PageBase.h"
#include "ElementInput.h"
class PageData
{
private:

protected:
public:
  inline static const char *inputName = "inputData";

  static ElementPage *create(Skin *skin)
  {
    ElementPage *pageData = new ElementPage("PageData",
                                            /* static func to exec  */ NULL, //&PageData::pageDataNum1,
                                            /* ms refrest seconds   */ 0,
                                            /* display Header       */ true,
                                            /* clear screen         */ true);

    Dimensions *dimensions;
    ElementButton *button;
    const char *icon;
    // bool addIcon = false;

    // button layout
    uint8_t across = 3;
    uint8_t down = 7;

    // =============================================================================
    // Add input
    dimensions = skin->getMapDimensions(across, down, 1, 2);
    //    pageData->addInput(new ElementInput("inputName", "some space for display", new Dimensions(
    pageData->addInput(new ElementInput(inputName, 18, new Dimensions(skin->buttonMargin, dimensions->getY(), skin->getScreenWidth() - (skin->buttonMargin * 2), dimensions->getH())));

    // =============================================================================
    // Add buttons: clear, Back, Send
    dimensions = skin->getMapDimensions(across, down, 1, 3);
    button = new ElementButton("Clear", dimensions, &PageData::pageClearButton, NULL, NULL, NULL, icon, "clr", false);
    pageData->addButton(button);

    // Add button remove last
    dimensions = skin->getMapDimensions(across, down, 2, 3);
    button = new ElementButton("Back", dimensions, &PageData::pageBackButton, NULL, NULL, NULL, icon, "bak", false);
    pageData->addButton(button);

    // Add button send
    dimensions = skin->getMapDimensions(across, down, 3, 3);
    button = new ElementButton("Send", dimensions, &PageData::pageSendButton, "PageOK", NULL, NULL /*, icon, "snd", false*/);
    pageData->addButton(button);

    // =============================================================================
    // Add button 1
    dimensions = skin->getMapDimensions(across, down, 1, 4);
    button = new ElementButton("1", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "1", false);
    pageData->addButton(button);

    // Add button 2
    dimensions = skin->getMapDimensions(across, down, 2, 4);
    button = new ElementButton("2", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "2", false);
    pageData->addButton(button);

    // Add button 3
    dimensions = skin->getMapDimensions(across, down, 3, 4);
    button = new ElementButton("3", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "3", false);
    pageData->addButton(button);

    // =============================================================================
    // Add button 4
    dimensions = skin->getMapDimensions(across, down, 1, 5);
    button = new ElementButton("4", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "4", false);
    pageData->addButton(button);

    // Add button 5
    dimensions = skin->getMapDimensions(across, down, 2, 5);
    button = new ElementButton("5", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "5", false);
    pageData->addButton(button);

    // Add button 6
    dimensions = skin->getMapDimensions(across, down, 3, 5);
    button = new ElementButton("6", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "6", false);
    pageData->addButton(button);

    // =============================================================================
    // Add button 7
    dimensions = skin->getMapDimensions(across, down, 1, 6);
    button = new ElementButton("7", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "7", false);
    pageData->addButton(button);

    // Add button 8
    dimensions = skin->getMapDimensions(across, down, 2, 6);
    button = new ElementButton("8", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "8", false);
    pageData->addButton(button);

    // Add button 9
    dimensions = skin->getMapDimensions(across, down, 3, 6);
    button = new ElementButton("9", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "9", false);
    pageData->addButton(button);

    // =============================================================================
    // Add button dot
    dimensions = skin->getMapDimensions(across, down, 1, 7);
    button = new ElementButton("*", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "*", false);
    pageData->addButton(button);

    // Add button 0
    dimensions = skin->getMapDimensions(across, down, 2, 7);
    button = new ElementButton("0", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "0", false);
    pageData->addButton(button);

    // Add button pound
    dimensions = skin->getMapDimensions(across, down, 3, 7);
    button = new ElementButton("#", dimensions, &PageData::pageNumButton, NULL, NULL, NULL, icon, "#", false);
    pageData->addButton(button);

    return pageData;
  }

  static void pageClearButton(const char *value)
  { // Do some work on a specific variables page
    ElementInput *inputValue = Menu::getInstance()->getPageInput(inputName);
    if (inputValue != NULL)
      inputValue->clear();
  }
  static void pageBackButton(const char *value)
  { // Do some work on a specific variables page
    ElementInput *inputValue = Menu::getInstance()->getPageInput(inputName);
    if (inputValue != NULL)
      inputValue->back();
  }
  static void pageSendButton(const char *value)
  { // Do some work on a specific variables page
    ElementInput *inputValue = Menu::getInstance()->getPageInput(inputName);
    if (inputValue != NULL)
      Serial.println(inputValue->getInput());
  }

  static void pageNumButton(const char *value)
  { // Do some work on a specific variables page
    ElementInput *inputValue = Menu::getInstance()->getPageInput(inputName);
    if (inputValue != NULL)
      inputValue->append(value);
  }
};

#endif
