/*
    This Page Element class provides the ability to
    1) execute static funtion on a schedule
      a) update screen variable fields (I.E. cnc current X, Y Z position)
      b) if needed, the function can change current Page)
    2) contain all the information about a page (Name, buttons, label, arguments, variables.)
*/

#ifndef ElementPage_h
#define ElementPage_h

#include "Arduino.h"

#include "LinkListPlus.h"

#include "ControlBase.h"
#include "ElementButton.h"
#include "ElementFile.h"
#include "ElementHeader.h"
#include "ElementInput.h"
#include "ElementLabel.h"
#include "ElementRectangle.h"
#include "ElementVariable.h"

#include "TBA_Macros.h"

#include "GlobalConst.h"
//#include "LCD.h"

// Required for ConvertString & ConvertChar
// #include <iostream>
#include <sstream>
#include <string>

class ElementPage : public ControlBase
{
private:
  ElementHeader *elementHeader;
  LinkListPlus *buttonListPlus = new LinkListPlus();
  LinkListPlus *inputListPlus = new LinkListPlus();
  LinkListPlus *labelListPlus = new LinkListPlus();
  LinkListPlus *rectangleListPlus = new LinkListPlus();
  LinkListPlus *variableListPlus = new LinkListPlus();

  boolean displayHeader;
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

  virtual boolean buttonShortPress(ElementButton *button) { return false; };
  virtual boolean buttonLongPress(ElementButton *button) { return false; };

  ElementPage(
      const char *name,
      uint16_t refreshTimer,
      boolean displayHeader,
      boolean clearScreen,
      uint8_t backPageDelay,
      const char *backPage) : ControlBase(name)
  {
    this->elementHeader = new ElementHeader(name, displayHeader);
    this->refreshTimer = refreshTimer;
    this->displayHeader = displayHeader;
    this->clrScreen = clearScreen;
    this->backPageDelay = backPageDelay;
    this->backPage = backPage;
  }

  ~ElementPage()
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

  void clearScreen()
  {
    if (this->clrScreen)
    { // Clear the screen
      tft.fillScreen(getLCD()->getSkin()->textBackgroundColor);
    }
  }

  void clearFiles()
  {
    this->buttonListPlus->setCurrentToHead();
    ElementButton *button = this->nextButton();
    while (button)
    {
      if (button->isStyleFile())
      {
        ElementFile *file = ((ElementFile *)button);
        this->buttonListPlus->deleteID(file->getIdentity());
      }

      // Get next button
      button = this->nextButton();
    }
  }

  void drawLabels()
  {
    this->labelListPlus->setCurrentToHead();
    ElementLabel *label = this->nextLabel();
    while (label)
    {
      label->draw();
      label = this->nextLabel();
    }
  }
  void drawVariables()
  {
    this->variableListPlus->setCurrentToHead();
    ElementVariable *variable = this->nextVariable();
    while (variable)
    {
      variable->draw();
      variable = this->nextVariable();
    }
  }
  void drawInputs()
  {
    this->inputListPlus->setCurrentToHead();
    ElementInput *input = this->nextInput();
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
    ElementInput *input = this->nextInput();
    while (input)
    {
      input->setInputChange();
      input = this->nextInput();
    }
  }
  void drawRectangles()
  {
    this->rectangleListPlus->setCurrentToHead();
    ElementRectangle *rectangle = this->nextRectangle();
    while (rectangle)
    {
      rectangle->draw();
      rectangle = this->nextRectangle();
    }
  }
  void drawFiles()
  {
    this->buttonListPlus->setCurrentToHead();
    ElementButton *button = this->nextButton();
    while (button)
    {
      if (button->isStyleFile())
      {
        ElementFile *file = (ElementFile *)button;
        file->draw();
      }
      button = this->nextButton();
    }
  }
  void drawButtons()
  {
    this->buttonListPlus->setCurrentToHead();
    ElementButton *button = this->nextButton();
    while (button)
    {
      if (button->isStyleButton())
      {
        button->draw();
      }
      button = this->nextButton();
    }
  }
  
  void drawPage()
  {
    // Clear the screen if required
    clearScreen();

    this->elementHeader->draw();

    drawRectangles();

    drawInputs();

    drawLabels();

    drawButtons();
    drawFiles();

    drawVariables();
  }

  boolean getDisplayHeader()
  {
    return this->displayHeader;
  }

  uint16_t addButton(ElementButton *elementButton)
  {
    return this->buttonListPlus->insertAtEnd((ElementBase *)elementButton);
  }
  uint16_t addFile(ElementFile *elementFile)
  {
    return this->buttonListPlus->insertAtEnd((ElementBase *)elementFile);
  }
  uint16_t addInput(ElementInput *inputElement)
  {
    return this->inputListPlus->insertAtEnd((ElementBase *)inputElement);
  }
  uint16_t addLabel(ElementLabel *labelElement)
  {
    return this->labelListPlus->insertAtEnd((ElementBase *)labelElement);
  }
  uint16_t addRectangle(ElementRectangle *rectangleElement)
  {
    return this->rectangleListPlus->insertAtEnd((ElementBase *)rectangleElement);
  }
  uint16_t addVariable(ElementVariable *variableElement)
  {
    return this->variableListPlus->insertAtEnd((ElementBase *)variableElement);
  }

  ElementInput *getPageInput(const char *name)
  {
    ElementInput *findInput = (ElementInput *)this->inputListPlus->searchName(name);
    if (findInput)
    {
      return findInput;
    }

    Serial.print("getPageInput not found: '");
    Serial.print(name);
    Serial.println("'");

    return NULL;
  }
  ElementRectangle *getPageRectangle(const char *name)
  {
    ElementRectangle *findRect = (ElementRectangle *)this->rectangleListPlus->searchName(name);
    if (findRect)
    {
      return findRect;
    }

    Serial.print("getPageRectangle not found: ");
    Serial.println(name);

    return NULL;
  }
  ElementVariable *getPageVariable(const char *name)
  {
    ElementVariable *findVariable = (ElementVariable *)this->variableListPlus->searchName(name);
    if (findVariable)
    {
      return findVariable;
    }

    Serial.print("getPageVariable not found: ");
    Serial.println(name);

    return NULL;
  }

  ElementButton *nextButton()
  {
    return (ElementButton *)this->buttonListPlus->getNext();
  }
  ElementInput *nextInput()
  {
    return (ElementInput *)this->inputListPlus->getNext();
  }
  ElementLabel *nextLabel()
  {
    return (ElementLabel *)this->labelListPlus->getNext();
  }
  ElementRectangle *nextRectangle()
  {
    return (ElementRectangle *)this->rectangleListPlus->getNext();
  }
  ElementVariable *nextVariable()
  {
    return (ElementVariable *)this->variableListPlus->getNext();
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
    ElementButton *button = (ElementButton *)nextButton();
    while (button)
    {
      if (button->isStyleFile())
        ((ElementFile *)button)->debugSerial("ElementFile * * ");
      else
        button->debugSerial("ElementButton");
      button = (ElementButton *)nextButton();
    }

    Serial.println("- - - - - - - - - -Element Input- - - - - - - - - -");
    ElementInput *input = nextInput();
    while (input)
    {
      input->debugSerial("ElementInput");
      input = nextInput();
    }

    Serial.println("- - - - - - - - - -Element Label- - - - - - - - - -");
    ElementLabel *label = nextLabel();
    while (label)
    {
      label->debugSerial("ElementLabel");
      label = nextLabel();
    }

    Serial.println("- - - - - - - - - -Element Rectangle- - - - - - - - - -");
    ElementRectangle *rectangle = nextRectangle();
    while (rectangle)
    {
      rectangle->debugSerial("ElementRectangle");
      rectangle = nextRectangle();
    }

    Serial.println("- - - - - - - - - -Element Variable- - - - - - - - - -");
    ElementVariable *variable = nextVariable();
    while (variable)
    {
      variable->debugSerial("traversLists - ElementVariable");
      variable = nextVariable();
    }
  }

  void debugSerial(const char *debugLocation)
  {
    Serial.print(F(" <"));
    Serial.print(__FILENAME__);
    Serial.print(F("> "));

    Serial.print("displayHeader: '");
    Serial.print(this->displayHeader);

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

    ControlBase::debugSerial(debugLocation);
  }
};

#endif
