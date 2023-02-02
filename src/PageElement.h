/*
    This Page Element class provides the ability to 
    1) execute static funtion on a schedule 
      a) update screen variable fields (I.E. cnc current X, Y Z position)
      b) if needed, the function can change current Page)
    2) contain all the information about a page (Name, buttons, label, arguments, variables.)
*/

#ifndef PageElement_h
#define PageElement_h

#include "Arduino.h"

#include "TBA_ListPlus.h"

#include "ButtonElement.h"
#include "InputElement.h"
#include "LabelElement.h"
#include "RectangleElement.h"
#include "VirtualBase.h"
#include "VariableElement.h"

//#include "../../TBA_Macros/src/TBA_Macros.h"
#include "TBA_Macros.h"

#include "GlobalConst.h"

class PageElement : public VirtualBase
{
private:
// in public, move here -  TBA_ListPlus<ButtonElement> *buttonListPlus = new TBA_ListPlus<ButtonElement>();
  TBA_ListPlus<InputElement> *inputListPlus = new TBA_ListPlus<InputElement>();  // used for screen updates for variable
  TBA_ListPlus<LabelElement> *labelListPlus = new TBA_ListPlus<LabelElement>();
  TBA_ListPlus<RectangleElement> *rectangleListPlus = new TBA_ListPlus<RectangleElement>();
  TBA_ListPlus<VariableElement> *variableListPlus = new TBA_ListPlus<VariableElement>();  // used for screen updates for variable
  // TBA_ListPlus<ArgumentElement> *argumentListPlus = new TBA_ListPlus<ArgumentElement>();  // used to pass to exec or next page

  boolean displayHeader;
  boolean clearScreen;

  uint8_t  refrestTimer = 1; // seconds
  uint64_t refreshNext  = 0;
  void (*pRefreshFunction)(void); // pointer to a function to manage the display outside the buttons

  // will be used if both fields are populated
  uint8_t backPageDelay; // Num of sec to redirect to backPage. Max 255 seconds
  std::string backPage;  //delayPage & delayPageSeconds



protected:
public:
  TBA_ListPlus<ButtonElement> *buttonListPlus = new TBA_ListPlus<ButtonElement>();


  PageElement(std::string name, void (*pRefreshFunction)(void), 
    uint16_t refrestTimer, boolean displayHeader, boolean clearScreen,
    uint8_t backPageDelay = 0, std::string backPage = EMPTY) : VirtualBase(name) //----->call base class
  {
    this->displayHeader = displayHeader;
    this->clearScreen  = clearScreen;
    this->pRefreshFunction = pRefreshFunction;   
    this->refrestTimer = refrestTimer;
    // Optional & defaulted
    this->backPageDelay = backPageDelay;
    this->backPage = backPage;    
  }

  ~PageElement()
  {    
    // delete buttonListPlus;
    // delete inputListPlus;
    // delete labelListPlus;
    // delete rectangleListPlus;
    // delete variableListPlus;
  }

  boolean getDisplayHeader()
  {
    return this->displayHeader;
  }
  boolean getClearScreen()
  {
    return this->clearScreen;
  }

  void addButton(ButtonElement buttonElement)
  {
    this->buttonListPlus->insertBottom(buttonElement);
  }
  void addInput(InputElement inputElement)
  {
    this->inputListPlus->insertBottom(inputElement);
  }  
  void addLabel(LabelElement labelElement)
  {
    this->labelListPlus->insertBottom(labelElement);
  }
  void addRectangle(RectangleElement rectangleElement)
  {
    this->rectangleListPlus->insertBottom(rectangleElement);
  } 
  void addVariable(VariableElement variableElement)
  {
    this->variableListPlus->insertBottom(variableElement);
  }    
  void resetIterators()
  {
    this->buttonListPlus->iteratorHead(); // Includes File
    this->inputListPlus->iteratorHead();
    this->labelListPlus->iteratorHead();
    this->rectangleListPlus->iteratorHead();
    this->variableListPlus->iteratorHead();
  }

  ButtonElement *nextButton()
  {
    return this->buttonListPlus->iteratorNext();
  }  
  InputElement *nextInput()
  {
    return this->inputListPlus->iteratorNext();
  }
  LabelElement *nextLabel()
  {
    return this->labelListPlus->iteratorNext();
  }
  RectangleElement *nextRectangle()
  {
    return this->rectangleListPlus->iteratorNext();
  }
  VariableElement *nextVariable()
  {
    return this->variableListPlus->iteratorNext();
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
          this->pRefreshFunction();  // do some work on a specific page
          return true;
        }
      }
      return false;
  }

  uint8_t getBackPageDelay()
  {
      return this->backPageDelay;
  }
  std::string getBackPage()
  {
      return this->backPage;
  }
  boolean hasBackPage()
  {
      if ( !this->backPage.empty())
        if (this->backPageDelay != 0)
          return true;
      return false;
  }

  std::string getPageVariable(std::string name)
  {
      VariableElement *findVariable = this->variableListPlus->searchName(name);
      if (findVariable)
      {        
        return findVariable->getValue();
      }

      Serial.print("getPageVariable not found: ");
      Serial.println(name.c_str());

      return EMPTY;
  }
  std::string getPageInput(std::string name)
  {
      InputElement *findInput = this->inputListPlus->searchName(name);
      if (findInput)
      {        
        return findInput->getInput();
      }

      Serial.print("getPageInput not found: ");
      Serial.println(name.c_str());

      return EMPTY;
  }
  RectangleElement *getPageRectangle(std::string name)
  {
      RectangleElement *findInput = this->rectangleListPlus->searchName(name);
      if (findInput)
      {        
        return findInput;
      }

      Serial.print("getPageRectangle not found: ");
      Serial.println(name.c_str());

      return NULL;
  }

  boolean setPageVariable(std::string name, std::string value)
  {
      VariableElement *findVariable = this->variableListPlus->searchName(name.c_str());
      if (findVariable)
      {
        findVariable->setValue(value);
        return true;
      }
      Serial.print("setPageVariable not found: ");
      Serial.println(name.c_str());
      Serial.println(" : ");
      Serial.println(value.c_str());

      return false;
  }
  boolean setPageInput(std::string name, std::string input)
  {
      InputElement *findInput = this->inputListPlus->searchName(name.c_str());
      if (findInput)
      {
        findInput->setInput(input);
        return true;
      }
      Serial.print("setPageInput not found: ");
      Serial.print(name.c_str());
      Serial.println(" : ");
      Serial.println(input.c_str());

      return false;
  }

  void clearFiles()
  {
      // Setup iterator & loop on buttons
      this->buttonListPlus->iteratorHead();
      ButtonElement *button = nextButton();

      while (button)
      {
        if (button->isStyleFile())
        {
          this->buttonListPlus->removeID(button->getIdentity());
//          delete button;
        }
        // Get next button
        button = nextButton();
      }
  }

  void debugSerial(std::string debugLocation)
  {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

      VirtualBase::debugSerial(debugLocation);
  }
};

#endif
