/*
  PageData.h - Library for PageData code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageData_h
#define PageData_h

#include "Arduino.h"

#include <sstream>
#include <string>

#include "Skin.h"

#include "Dimensions.h"

#include "ControlButton.h"
#include "ControlInput.h"

#include "TBA_SupportFunction.h"
TBA_SupportFunction supportFunction = TBA_SupportFunction();

class PageData : public ControlPage
{
private:
  inline static const char *BACK = "Back";
  inline static const char *CLEAR = "Clear";
  inline static const char *SEND = "Send";
  inline static const char *PAGEOK = "PageOK";

protected:
public:
  inline static const char *NAME = "PageData";
  inline static const char *inputName = "inputData";

  PageData() : ControlPage(PageData::NAME, 0, true, true, 0, NULL, NULL)
  {
    Dimensions *dimensions;
    const char *icon;

    // button layout
    uint8_t across = 3;
    uint8_t down = 7;

    // =============================================================================
    // Add input
    dimensions = skin->getMapDimensions(across, down, 1, 2);
    this->addInput(new ControlInput(inputName, 18, ControlInput::JUSTIFICATION::RIGHT, new Dimensions(skin->buttonMargin, dimensions->getY(), skin->getScreenWidth() - (skin->buttonMargin * 2), dimensions->getH())));

    // =============================================================================
    // Add buttons: clear, Back, Send
    dimensions = skin->getMapDimensions(across, down, 1, 3);
    this->addButton(new ControlButton(PageData::CLEAR, dimensions, NULL, NULL, "clr"));

    // Add button remove last
    dimensions = skin->getMapDimensions(across, down, 2, 3);
    this->addButton(new ControlButton(PageData::BACK, dimensions, NULL, NULL, "bak"));

    // Add button send
    dimensions = skin->getMapDimensions(across, down, 3, 3);
    this->addButton(new ControlButton(PageData::SEND, dimensions, PAGEOK, NULL, "snd"));

    // =============================================================================
    // Add button 1
    dimensions = skin->getMapDimensions(across, down, 1, 4);
    this->addButton(new ControlButton("1", dimensions, NULL, NULL, "1"));

    // Add button 2
    dimensions = skin->getMapDimensions(across, down, 2, 4);
    this->addButton(new ControlButton("2", dimensions, NULL, NULL, "2"));

    // Add button 3
    dimensions = skin->getMapDimensions(across, down, 3, 4);
    this->addButton(new ControlButton("3", dimensions, NULL, NULL, "3"));

    // =============================================================================
    // Add button 4
    dimensions = skin->getMapDimensions(across, down, 1, 5);
    this->addButton(new ControlButton("4", dimensions, NULL, NULL, "4"));

    // Add button 5
    dimensions = skin->getMapDimensions(across, down, 2, 5);
    this->addButton(new ControlButton("5", dimensions, NULL, NULL, "5"));

    // Add button 6
    dimensions = skin->getMapDimensions(across, down, 3, 5);
    this->addButton(new ControlButton("6", dimensions, NULL, NULL, "6"));

    // =============================================================================
    // Add button 7
    dimensions = skin->getMapDimensions(across, down, 1, 6);
    this->addButton(new ControlButton("7", dimensions, NULL, NULL, "7"));

    // Add button 8
    dimensions = skin->getMapDimensions(across, down, 2, 6);
    this->addButton(new ControlButton("8", dimensions, NULL, NULL, "8"));

    // Add button 9
    dimensions = skin->getMapDimensions(across, down, 3, 6);
    this->addButton(new ControlButton("9", dimensions, NULL, NULL, "9"));

    // =============================================================================
    // Add button dot
    dimensions = skin->getMapDimensions(across, down, 1, 7);
    this->addButton(new ControlButton("*", dimensions, NULL, NULL, "*"));

    // Add button 0
    dimensions = skin->getMapDimensions(across, down, 2, 7);
    this->addButton(new ControlButton("0", dimensions, NULL, NULL, "0"));

    // Add button pound
    dimensions = skin->getMapDimensions(across, down, 3, 7);
    this->addButton(new ControlButton("#", dimensions, NULL, NULL, "#"));
  }

  void load()
  {
    supportFunction.memoryInfo();
  }

  void pageClearButton()
  {
    ControlInput *inputValue = getPageInput(PageData::inputName);
    if (inputValue)
      inputValue->clear();
  }

  void pageBackButton()
  {
    ControlInput *inputValue = getPageInput(PageData::inputName);
    if (inputValue)
      inputValue->back();
  }

  void pageSendButton()
  {
    ControlInput *inputValue = getPageInput(PageData::inputName);
    if (inputValue)
      Serial.print(" - - inputValue->getInput(): '");
      Serial.print(inputValue->getInput());
      Serial.println("'");
  }

  void pageNumButton(const char *value)
  {
    ControlInput *inputValue = getPageInput(PageData::inputName);
    if (inputValue)
      inputValue->append(value);
  }

  boolean buttonShortPress(ControlButton * button)
  {
    if (strcmp(SEND, button->getName()) == 0)
      pageSendButton();
    else if (strcmp(BACK, button->getName()) == 0)
      pageBackButton();
    else if (strcmp(CLEAR, button->getName()) == 0)
      pageClearButton();
    else
      pageNumButton(button->getValue()); // only button type left is data

    // ControlPage::buttonShortPress(button);// Points to virtual method that returns false

    return false; // Redraw page
  }

  boolean buttonLongPress(ControlButton * button)
  {
    Serial.print("buttonLongPress overload - name '");
    Serial.print(button->getName());
    Serial.print("' value '");
    Serial.print(button->getValue());
    Serial.println("'");

    // ControlPage::buttonLongPress(button);// Points to virtual method that returns false

    return false; // Redraw page 
  }
};

#endif
