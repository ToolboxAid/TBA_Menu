/*
    This Page Element class provides the ability to
    1) execute static funtion on a schedule
      a) update screen variable fields (I.E. cnc current X, Y Z position)
      b) if needed, the function can change current Page)
    2) contain all the information about a page (Name, buttons, label, arguments, variables.)
*/

#ifndef ControlPage_h
#define ControlPage_h

#include "Arduino.h"

#include "LinkListPlus.h"

#include "ControlBase.h"
#include "ControlButton.h"
#include "ControlFile.h"
#include "ControlHeader.h"
#include "ControlInput.h"
#include "ControlLabel.h"
#include "ControlRectangle.h"
#include "ControlVariable.h"

#include "TBA_Macros.h"

// Required for ConvertString & ConvertChar
// #include <iostream>
#include <sstream>
#include <string>

class ControlPage : public ControlBase
{
private:
  ControlHeader *controlHeader;
  LinkListPlus *buttonListPlus = new LinkListPlus();
  LinkListPlus *inputListPlus = new LinkListPlus();
  LinkListPlus *labelListPlus = new LinkListPlus();
  LinkListPlus *rectangleListPlus = new LinkListPlus();
  LinkListPlus *variableListPlus = new LinkListPlus();

  boolean clrScreen;

  uint8_t refreshTimer = 1; // seconds
  uint64_t refreshNext = 0;

  uint8_t backPageDelay; // Num of sec to redirect to backPage. Max 255 seconds

protected:
  const char *backPage; // delayPage & delayPageSeconds

public:
  virtual void load(){};
  virtual void refresh(){};
  virtual void exit(){};

  virtual boolean buttonShortPress(ControlButton *button) { return false; };
  virtual boolean buttonLongPress(ControlButton *button) { return false; };

  ControlPage(
      const char *name,
      uint16_t refreshTimer,
      boolean headerVisible,
      boolean clearScreen,
      uint8_t backPageDelay,
      const char *backPage,
      const char *headerIcon) : ControlBase(name)
  {
    this->controlHeader = new ControlHeader(name, headerVisible, headerIcon);
    this->refreshTimer = refreshTimer;
    this->clrScreen = clearScreen;
    this->backPageDelay = backPageDelay;
    this->backPage = backPage;
  }

  ~ControlPage()
  {
    // TODO: Below not complete, does it need to be as we are not deleting pages yet?
    //  delete buttonListPlus;
    //  delete inputListPlus;
    //  delete labelListPlus;
    //  delete rectangleListPlus;
    //  delete variableListPlus;
  }

  void resetButtonIterator()
  {
        this->buttonListPlus->setCurrentToHead();
  }

  void clearScreen(uint16_t color)
  {
    if (this->clrScreen)
    { // Clear the screen
      tft.fillScreen(color);
    }
  }

  void clearFiles()
  {
    this->buttonListPlus->setCurrentToHead();
    ControlButton *button = this->nextButton();
    while (button)
    {
      if (button->isStyleFile())
      {
        ControlFile *file = ((ControlFile *)button);
        this->buttonListPlus->deleteID(file->getIdentity());
      }

      // Get next button
      button = this->nextButton();
    }
  }

  void drawLabels()
  {
    this->labelListPlus->setCurrentToHead();
    ControlLabel *label = this->nextLabel();
    while (label)
    {
      label->draw();
      label = this->nextLabel();
    }
  }
  void drawVariables()
  {
    this->variableListPlus->setCurrentToHead();
    ControlVariable *variable = this->nextVariable();
    while (variable)
    {
      variable->draw();
      variable = this->nextVariable();
    }
  }
  void drawInputs()
  {
    this->inputListPlus->setCurrentToHead();
    ControlInput *input = this->nextInput();
    while (input)
    {
      if (input->getInputChange())
      {
        input->resetInputChange();
        input->draw();
      }
      input = this->nextInput();
    }
  }
  void setInputs()
  {
    this->inputListPlus->setCurrentToHead();
    // iterator
    ControlInput *input = this->nextInput();
    while (input)
    {
      input->setInputChange();
      input = this->nextInput();
    }
  }
  void drawRectangles()
  {
    this->rectangleListPlus->setCurrentToHead();
    ControlRectangle *rectangle = this->nextRectangle();
    while (rectangle)
    {
      rectangle->draw();
      rectangle = this->nextRectangle();
    }
  }
  void drawFiles()
  {
    this->buttonListPlus->setCurrentToHead();
    ControlButton *button = this->nextButton();
    while (button)
    {
      if (button->isStyleFile())
      {
        ControlFile *file = (ControlFile *)button;
        file->draw();
      }
      button = this->nextButton();
    }
  }
  void drawButtons()
  {
    this->buttonListPlus->setCurrentToHead();
    ControlButton *button = this->nextButton();
    while (button)
    {
      if (button->isStyleButton())
      {
        button->draw();
      }
      button = this->nextButton();
    }
  }
  
  void drawPage(uint16_t color)
  {
    // Clear the screen if required
    clearScreen(color);

    this->controlHeader->draw();

    drawRectangles();

    drawInputs();

    drawLabels();

    drawButtons();
    drawFiles();

    drawVariables();
  }

  boolean isVisible()
  {
    return this->controlHeader->isVisible();
  }

  uint16_t addButton(ControlButton *button)
  {
    return this->buttonListPlus->insertAtEnd((ElementBase *)button);
  }
  uint16_t addFile(ControlFile *file)
  {
    return this->buttonListPlus->insertAtEnd((ElementBase *)file);
  }
  uint16_t addInput(ControlInput *input)
  {
    return this->inputListPlus->insertAtEnd((ElementBase *)input);
  }
  uint16_t addLabel(ControlLabel *label)
  {
    return this->labelListPlus->insertAtEnd((ElementBase *)label);
  }
  uint16_t addRectangle(ControlRectangle *rectangle)
  {
    return this->rectangleListPlus->insertAtEnd((ElementBase *)rectangle);
  }
  uint16_t addVariable(ControlVariable *variable)
  {
    return this->variableListPlus->insertAtEnd((ElementBase *)variable);
  }

  ControlInput *getPageInput(const char *name)
  {
    ControlInput *findInput = (ControlInput *)this->inputListPlus->searchName(name);
    if (findInput)
    {
      return findInput;
    }

    Serial.print("getPageInput not found: '");
    Serial.print(name);
    Serial.println("'");

    return NULL;
  }
  ControlRectangle *getPageRectangle(const char *name)
  {
    ControlRectangle *findRect = (ControlRectangle *)this->rectangleListPlus->searchName(name);
    if (findRect)
    {
      return findRect;
    }

    Serial.print("getPageRectangle not found: ");
    Serial.println(name);

    return NULL;
  }
  ControlVariable *getPageVariable(const char *name)
  {
    ControlVariable *findVariable = (ControlVariable *)this->variableListPlus->searchName(name);
    if (findVariable)
    {
      return findVariable;
    }

    Serial.print("getPageVariable not found: ");
    Serial.println(name);

    return NULL;
  }

  ControlButton *nextButton()
  {
    return (ControlButton *)this->buttonListPlus->getNext();
  }
  ControlInput *nextInput()
  {
    return (ControlInput *)this->inputListPlus->getNext();
  }
  ControlLabel *nextLabel()
  {
    return (ControlLabel *)this->labelListPlus->getNext();
  }
  ControlRectangle *nextRectangle()
  {
    return (ControlRectangle *)this->rectangleListPlus->getNext();
  }
  ControlVariable *nextVariable()
  {
    return (ControlVariable *)this->variableListPlus->getNext();
  }

  boolean removeInput(uint16_t identity)
  {
    return this->inputListPlus->deleteID(identity);
  }
  boolean removeLabel(uint16_t identity)
  {
    return this->labelListPlus->deleteID(identity);
  }
  boolean removePageRectangle(const char *name)
  {
    return this->rectangleListPlus->deleteName(name);
  }

  boolean checkRefresh()
  {
    if (this->refreshTimer != 0 && millis() >= this->refreshNext)
    {
      uint64_t lastRefreshTime = this->refreshNext;
      this->refreshNext = millis() + (this->refreshTimer * 1000);

      if (lastRefreshTime > this->refreshNext) // check for roleover (I've read it takes ~49 Days, I know, overly cautious)
      {
        this->refreshNext = (this->refreshTimer * 1000);
        Serial.println("checkRefresh(overflow)");
      }

      this->refresh();
      return true;
    }
    return false;
  }

  boolean hasBackPage()
  {
    if (this->backPage && this->backPage[0] != '\0')
      if (this->backPageDelay != 0)
        return true;
    return false;
  }
  const char *getBackPage()
  {
    return this->backPage;
  }
  uint8_t getBackPageDelay()
  {
    return this->backPageDelay;
  }

  /* Formating options...
  Format	Type    bytes
  ======  ======  =====
  %c   	char      1/char
  %i  	int       4
  %u  	unsigned  4
  %f  	float     4
        double    8
  %s  	string    1/char + 1 for terminater
  */
  /* Tested with 'unsigned short int', 'int', float, double */
  template <typename T>
  T ConvertString(const std::string &name, const std::string &data, boolean &success)
  {
    success = true;
    if (!data.empty())
    {
      T ret;
      std::istringstream iss(data);
      if (data.find("0x") != std::string::npos)
      {
        iss >> std::hex >> ret;
      }
      else
      {
        iss >> std::dec >> ret;
      }

      if (iss.fail())
      {
        success = false;
        Serial.print("ConvertString failed for varName: '");
        Serial.print(name.c_str());
        Serial.print("' value: '");
        Serial.print(data.c_str());
        Serial.println("' '");
        return T();
      }
      return ret;
    }
    return T();
  }

  template <typename T>
  T ConvertChar(const char *nameIn, const char *dataIn, boolean &success)
  {
    success = true;
    std::string data(dataIn);

    if (!data.empty())
    {
      T ret;
      std::istringstream iss(data);
      if (data.find("0x") != std::string::npos)
      {
        iss >> std::hex >> ret;
      }
      else
      {
        iss >> std::dec >> ret;
      }

      if (iss.fail())
      {
        success = false;
        Serial.print("ConvertChar failed for varName: '");
        Serial.print(nameIn);
        Serial.print("' value: '");
        Serial.print(dataIn);
        Serial.println("'");
        return T();
      }
      return ret;
    }
    return T();
  }

  void traverPageLists()
  { // Page owned LinkedLists
    resetButtonIterator();

    Serial.println("- - - - - - - - - -Element Button- - - - - - - - - -");
    ControlButton *button = (ControlButton *)nextButton();
    while (button)
    {
      if (button->isStyleFile())
        ((ControlFile *)button)->debugSerial("ControlFile * * ");
      else
        button->debugSerial("ControlButton");
      button = (ControlButton *)nextButton();
    }

    Serial.println("- - - - - - - - - -Element Input- - - - - - - - - -");
    ControlInput *input = nextInput();
    while (input)
    {
      input->debugSerial("ControlInput");
      input = nextInput();
    }

    Serial.println("- - - - - - - - - -Element Label- - - - - - - - - -");
    ControlLabel *label = nextLabel();
    while (label)
    {
      label->debugSerial("ControlLabel");
      label = nextLabel();
    }

    Serial.println("- - - - - - - - - -Element Rectangle- - - - - - - - - -");
    ControlRectangle *rectangle = nextRectangle();
    while (rectangle)
    {
      rectangle->debugSerial("ControlRectangle");
      rectangle = nextRectangle();
    }

    Serial.println("- - - - - - - - - -Element Variable- - - - - - - - - -");
    ControlVariable *variable = nextVariable();
    while (variable)
    {
      variable->debugSerial("traversLists - ControlVariable");
      variable = nextVariable();
    }
  }

  void debugSerial(const char *debugLocation)
  {
    Serial.print(F(" <"));
    Serial.print(__FILENAME__);
    Serial.print(F("> "));


    Serial.print("' clearScreen: '");
    Serial.print(this->clrScreen);

    Serial.print("' refreshTimer: '");
    Serial.print(this->refreshTimer);

    Serial.print("' refreshNext: '");
    Serial.print(this->refreshNext);

    // will be used if both fields are populated
    Serial.print("' backPageDelay: '");
    Serial.print(this->backPageDelay);
    Serial.print("' backPage: '");
    Serial.print(this->backPage);

    Serial.print("' ");

    this->controlHeader->debugSerial("NO_CR");
    ControlBase::debugSerial(debugLocation);
  }
};

#endif
