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

#include "ElementButton.h"
#include "ElementInput.h"
#include "ElementLabel.h"
#include "ElementRectangle.h"
#include "ElementBase.h"
#include "ElementVariable.h"

#include "TBA_Macros.h"

#include "GlobalConst.h"

class ElementPage : public ElementBase
{
private:
  // in public, move here -  LinkListPlus *buttonListPlus = new LinkListPlus();
  LinkListPlus *inputListPlus = new LinkListPlus(); // used for screen updates for variable
  LinkListPlus *labelListPlus = new LinkListPlus();
  LinkListPlus *rectangleListPlus = new LinkListPlus();
  LinkListPlus *variableListPlus = new LinkListPlus(); // used for screen updates for variable
  // LinkListPlus *argumentListPlus = new LinkListPlus();  // used to pass to exec or next page

  boolean displayHeader;
  boolean clearScreen;

  void (*pLoadFunction)(void);    // pointer to a function to manage the display outside the buttons
  void (*pRefreshFunction)(void); // pointer to a function to manage the display outside the buttons
  uint8_t refrestTimer = 1;       // seconds
  uint64_t refreshNext = 0;
  void (*pExitFunction)(void); // pointer to a function to manage the display outside the buttons

  // will be used if both fields are populated
  uint8_t backPageDelay; // Num of sec to redirect to backPage. Max 255 seconds
  const char *backPage;  // delayPage & delayPageSeconds

protected:
public:
  LinkListPlus *buttonListPlus = new LinkListPlus(); // This needs moved back to private:

  ElementPage(
      const char *name,
      void (*pRefreshFunction)(void),
      uint16_t refrestTimer,
      boolean displayHeader,
      boolean clearScreen) : ElementBase(name) //----->call base class
  {
    this->displayHeader = displayHeader;
    this->clearScreen = clearScreen;
    this->pLoadFunction = NULL;
    this->pRefreshFunction = pRefreshFunction;
    this->refrestTimer = refrestTimer;
    this->pExitFunction = NULL;

    this->backPageDelay = 0;
    this->backPage = NULL;
  }

  ElementPage(
      const char *name,
      void (*pRefreshFunction)(void),
      uint16_t refrestTimer,
      boolean displayHeader,
      boolean clearScreen,
      uint8_t backPageDelay,
      const char *backPage) : ElementBase(name) //----->call base class
  {
    this->displayHeader = displayHeader;
    this->clearScreen = clearScreen;
    this->pLoadFunction = NULL;
    this->pRefreshFunction = pRefreshFunction;
    this->refrestTimer = refrestTimer;
    this->pExitFunction = NULL;
    this->backPageDelay = backPageDelay;
    this->backPage = backPage;
  }

  ElementPage(
      const char *name,
      void (*pLoadFunction)(void),
      void (*pRefreshFunction)(void),
      uint16_t refrestTimer,
      void (*pExitFunction)(void),
      boolean displayHeader,
      boolean clearScreen,
      uint8_t backPageDelay,
      const char *backPage) : ElementBase(name) //----->call base class
  {
    this->displayHeader = displayHeader;
    this->clearScreen = clearScreen;
    this->pLoadFunction = pLoadFunction;
    this->pRefreshFunction = pRefreshFunction;
    this->refrestTimer = refrestTimer;
    this->pExitFunction = pExitFunction;
    this->backPageDelay = backPageDelay;
    this->backPage = backPage;
  }

  ~ElementPage()
  {
    // delete buttonListPlus;
    // delete inputListPlus;
    // delete labelListPlus;
    // delete rectangleListPlus;
    // delete variableListPlus;
  }

  void doExit()
  {
    Serial.println("Load");
    // if (this->pLoadFunction != NULL)
    //   this->pLoadFunction();
  }
  void doLoad()
  {
    Serial.println("Exit");
    // if (this->pExitFunction != NULL)
    //   this->pExitFunction();
  }

  boolean getDisplayHeader()
  {
    return this->displayHeader;
  }
  boolean getClearScreen()
  {
    return this->clearScreen;
  }

  void addButton(ElementButton *elementButton)
  {
    ElementBase *elementBase = (ElementBase *)elementButton;
    this->buttonListPlus->insertAtEnd(elementBase);
  }
  void addFile(ElementButton *elementFile)
  {
    ElementBase *elementBase = (ElementBase *)elementFile;
    this->buttonListPlus->insertAtEnd(elementBase);
  }
  void addInput(ElementInput *inputElement)
  {
    this->inputListPlus->insertAtEnd((ElementBase *)inputElement);
  }
  void addLabel(ElementLabel *labelElement)
  {
    this->labelListPlus->insertAtEnd((ElementBase *)labelElement);
  }
  void addRectangle(ElementRectangle *rectangleElement)
  {
    this->rectangleListPlus->insertAtEnd((ElementBase *)rectangleElement);
  }
  void addVariable(ElementVariable *variableElement)
  {
    this->variableListPlus->insertAtEnd((ElementBase *)variableElement);
  }
  void resetIterators()
  {
    this->buttonListPlus->setCurrentToHead(); // Includes File
    this->inputListPlus->setCurrentToHead();
    this->labelListPlus->setCurrentToHead();
    this->rectangleListPlus->setCurrentToHead();
    this->variableListPlus->setCurrentToHead();
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

  boolean checkRefresh()
  {
    if (millis() >= this->refreshNext)
    {
      uint64_t lastRefreshTime = this->refreshNext;
      this->refreshNext = millis() + (refrestTimer * 1000);
      // check for roleover (I've read it takes ~49 Days)
      if (lastRefreshTime > this->refreshNext)
      {
        this->refreshNext = (refrestTimer * 1000);
      }
      // check if refreshFunction available
      if (this->pRefreshFunction != NULL)
      {
        this->pRefreshFunction(); // do some work on a specific page
        return true;
      }
    }
    return false;
  }

  uint8_t getBackPageDelay()
  {
    return this->backPageDelay;
  }
  const char *getBackPage()
  {
    return this->backPage;
  }
  boolean hasBackPage()
  {
    if (this->backPage != NULL)
      if (this->backPageDelay != 0)
        return true;
    return false;
  }

  ElementVariable *getPageVariable(const char *name)
  {
    ElementVariable *findVariable = (ElementVariable *)this->variableListPlus->searchName(name);
    if (findVariable)
    {
      // return findVariable->getValue();
      return findVariable;
    }

    Serial.print("getPageVariable not found: ");
    Serial.println(name);

    return NULL;
  }
  ElementInput *getPageInput(const char *name)
  {
    ElementInput *findInput = (ElementInput *)this->inputListPlus->searchName(name);
    if (findInput)
    {
      return findInput;
    }

    Serial.print("getPageInput not found: ");
    Serial.println(name);

    return NULL;
  }
  ElementRectangle *getPageRectangle(const char *name)
  {
    ElementRectangle *findInput = (ElementRectangle *)this->rectangleListPlus->searchName(name);
    if (findInput)
    {
      return findInput;
    }

    Serial.print("getPageRectangle not found: ");
    Serial.println(name);

    return NULL;
  }

  void clearFiles()
  {
    // Setup iterator & loop on buttons
    this->buttonListPlus->setCurrentToHead();
    ElementButton *button = nextButton();

    while (button)
    {
      if (button->isStyleFile())
      {
        this->buttonListPlus->deleteID(button->getIdentity());
        //          delete button;
      }
      // Get next button
      button = nextButton();
    }
  }

  void debugSerial(const char *debugLocation)
  {
    Serial.print(F(" <"));
    Serial.print(__FILENAME__);
    Serial.print(F("> "));

    Serial.print("displayHeader: '");
    Serial.print(displayHeader);

    Serial.print("' clearScreen: '");
    Serial.print(clearScreen);

    Serial.print("' refrestTimer: '");
    Serial.print(refrestTimer);

    Serial.print("' refreshNext: '");
    Serial.print(refreshNext);

    Serial.print("' pRefreshFunction: '");
    Serial.print((int)pRefreshFunction, HEX);

    // will be used if both fields are populated
    Serial.print("' backPageDelay: '");
    Serial.print(backPageDelay);

    Serial.print("' backPage: '");
    Serial.print(backPage);
    Serial.print("' ");

    ElementBase::debugSerial(debugLocation);
  }
};

#endif
