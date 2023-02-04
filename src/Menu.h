/*
  Menu.h - Library for Menu code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

/*
Other than ILI9431, the library also supports:
ILI9341_2, ST7735, ILI9163, S6D02A1, RPI_ILI9486,
HX8357D, ILI9481, ILI9486, ILI9488, ST7789, ST7789_2,
R61581, RM68140, ST7796, SSD1351, SSD1963_480,
SSD1963_800, SSD1963_800ALT, ILI9225, GC9A01.
*/

#pragma once

#ifndef Menu_h
#define Menu_h

#include "Arduino.h"

/* The SPIFFS (FLASH file system) is used
   to hold touch screen calibration data
#include "TBA_FileSystem.h"
*/

#include "Display.h"

#include "FS.h"
#include <SPI.h>

#include "Point.h"
#include "Skin.h"

#include "ElementBase.h"

#include "ElementPage.h"
#include "ElementButton.h"
#include "ElementFile.h"
#include "ElementRectangle.h"
#include "LinkListPlus.h"

#include "PageTBA.h"

/* This is a singleton class,
only one instance will ever be created
*/
class Menu
{
private:
  static Menu *thisMenu;

  Menu() {}
  boolean executeShortButtonMethod();
  boolean executeLongButtonMethod();
  boolean executePageBackMethod();
  boolean checkPageChange();

  LinkListPlus *pageListPlus = new LinkListPlus();

  unsigned long pageLoadTime = 0;
  ElementPage *currentPage = NULL;
  ElementPage *newPage = NULL;

  // Store the touch coordinates
  uint16_t t_x = 0, t_y = 0;
  Point *point;

protected:
public:
  static Menu *getInstance()
  {
    if (!thisMenu)
    {
      thisMenu = new Menu; // ()
    }
    return thisMenu;
  }

  void init(Skin &skin, const char *frontPage); /*---------------------*/

  void addPage(ElementPage *page);

  void checkMenuActions();

  ElementVariable *getPageVariable(const char *name);

  ElementInput *getPageInput(const char *name);

  ElementRectangle *getPageRectangle(const char *name);

  // void clearFiles(const char* endsWith);
  // bool addFile(ElementFile *file);

  ElementPage *getCurrentPage();

  void debugSerial(const char *debugLocation);
};

/* Allocating and initializing GlobalClass's
   static data member.  The pointer is being
   allocated - not the object inself. */

Menu *Menu::thisMenu = 0;

void Menu::init(Skin &skin, const char *frontPage = "Main")
{
  Display::getInstance()->init(skin); /*---------------------*/

  // Add TBA Page & Set to current
  ElementPage *tbaPage = PageTBA::create(frontPage, skin.getScreenWidth());
  addPage(tbaPage);
  thisMenu->currentPage = tbaPage;

  thisMenu->newPage = thisMenu->currentPage;
  thisMenu->currentPage = NULL;
}

void Menu::addPage(ElementPage *page)
{
  thisMenu->pageListPlus->insertAtEnd(((ElementBase *)page));
}

boolean Menu::executePageBackMethod()
{
  if (thisMenu->currentPage->hasBackPage())
  {
    if (millis() > (thisMenu->pageLoadTime + (thisMenu->currentPage->getBackPageDelay() * 1000)))
    {
      ElementPage *findPage = (ElementPage *)thisMenu->pageListPlus->searchName(thisMenu->currentPage->getBackPage());
      if (findPage)
      {
        thisMenu->newPage = findPage;
        return true;
      }
      else
      {
        thisMenu->pageLoadTime = millis(); // set time to prevent spam
        Serial.print("executePageBackMethod change not found: '");
        Serial.print(thisMenu->currentPage->getBackPage());
        Serial.print("' backPageDelay: '");
        Serial.print(thisMenu->currentPage->getBackPageDelay());
        Serial.println("'");
      }
    }
  }
  return false;
}
bool Menu::executeShortButtonMethod()
{
  ElementPage *findPage = (ElementPage *)thisMenu->pageListPlus->searchName(thisMenu->currentPage->getBackPage());
  if (findPage)
  {
    thisMenu->newPage = findPage;
    return true;
  }
  else
  {
    thisMenu->pageLoadTime = millis(); // set time to prevent spam
    Serial.print("executeShortButtonMethod change not found: ");
    Serial.println(thisMenu->currentPage->getBackPage());
  }

  return false;
}
bool Menu::executeLongButtonMethod()
{
  ElementPage *findPage = (ElementPage *)thisMenu->pageListPlus->searchName(thisMenu->currentPage->getBackPage());
  if (findPage)
  {
    thisMenu->newPage = findPage;
    return true;
  }
  else
  {
    thisMenu->pageLoadTime = millis(); // set time to prevent spam
    Serial.print("executeLongButtonMethod change not found: ");
    Serial.println(thisMenu->currentPage->getBackPage());
  }

  return false;
}

boolean Menu::checkPageChange()
{
  if (thisMenu->newPage != thisMenu->currentPage)
  {
    thisMenu->currentPage = thisMenu->newPage;
    thisMenu->pageLoadTime = millis(); // set page load time
    Display::getInstance()->drawCurrentPage(thisMenu->currentPage);
    return true;
  }
  return false;
}

void Menu::checkMenuActions()
{ // Check page actions (update screen based on current page, buttons, timer, event, ???)
  if (thisMenu->checkPageChange())
    return; // new page, not worth the risk of a delayed updates.

  if (thisMenu->executePageBackMethod())
    return; // new page, not worth the risk of a delayed updates.

  // Iterate of button to check if point is pressed....
  boolean pressed;
  Point *point = Display::getInstance()->getScreenTouchPoint(pressed);

  thisMenu->currentPage->resetIterators();
  ElementButton *button = thisMenu->currentPage->nextButton();
  while (button)
  {
    // action of short or long, change page
    ElementButton::STATE triggerState = ElementButton::STATE::UP;

    if (button->hasStateChange(point, pressed, triggerState)) // Check if button::State changed
    {
      Display::getInstance()->drawButton(thisMenu->currentPage, button);
    }
    if (button->isRELEASED()) // we are changing page, short or long?
    {
      // use action to change page
      button->resetButton();

      ElementPage *findPage;

      if (triggerState == ElementButton::STATE::SHORT)
      { // Short
        button->executeShortFunction();

        findPage = (ElementPage *)thisMenu->pageListPlus->searchName(button->getShortPage());
        if (findPage)
        {
          thisMenu->newPage = findPage;
          return; // new page, not worth the risk of a delayed updates.
        }
        else
        {
          thisMenu->pageLoadTime = millis(); // set time to prevent spam
          if (!button->getShortPage() == NULL)
          {
            Serial.println(button->getShortPage());
            Serial.print("checkMenuActions short button change page not found: '");
            Serial.print(button->getShortPage());
            Serial.println("'");
            //
            //
            Serial.println("Valid Pages are: ");
            pageListPlus->setCurrentToHead();
            ElementPage *page = (ElementPage *)pageListPlus->getNext();
            while (page)
            {
              // page->debugSerial("all pages");
              Serial.print("ID: '");
              Serial.print(page->getIdentity());
              Serial.print("' Name: '");
              Serial.print(page->getName());
              Serial.print("'");
              Serial.println();
              page = (ElementPage *)pageListPlus->getNext();
            }
            Serial.print("' Size: '");
            Serial.print(pageListPlus->getNodeCount());
            Serial.print("'");
            Serial.println();

            //
            //
          }
        }
      }
      else
      { // Long
        button->executeLongFunction();
        findPage = (ElementPage *)thisMenu->pageListPlus->searchName(button->getLongPage());
        if (findPage)
        {
          thisMenu->newPage = findPage;
          return; // new page, not worth the risk of a delayed updates.
        }
        else
        {
          thisMenu->pageLoadTime = millis(); // set time to prevent spam
          if (!button->getLongPage() == NULL)
          {
            Serial.print("checkMenuActions long button change page not found: '");
            Serial.print(button->getLongPage());
            Serial.println("'");
          }
        }
      }
    }
    if (button->isROLLOFF())
    {
      button->resetButton();
    }

    button = thisMenu->currentPage->nextButton();
  }

  if (thisMenu->currentPage->checkRefresh())
  {
    Display::getInstance()->drawVariable(thisMenu->currentPage);
  }
  else
  {
    Display::getInstance()->checkInput(thisMenu->currentPage);
  }

  // delete anything we use NEW on.
  delete point;
}

ElementVariable *Menu::getPageVariable(const char *name)
{ // This is the only way to get access to current page
  return thisMenu->currentPage->getPageVariable(name);
}

ElementInput *Menu::getPageInput(const char *name)
{ // This is the only way to get access to current page
  return thisMenu->currentPage->getPageInput(name);
}

ElementRectangle *Menu::getPageRectangle(const char *name)
{ // This is the only way to get access to current page
  return thisMenu->currentPage->getPageRectangle(name);
}

/*
void Menu::clearFiles(const char* endsWith)
{
 Serial.println("clearFiles"); delay(1000);
   // Setup iterator & loop on buttons
  ElementButton *button = currentPage->nextButton();

  while (button)
  {
    //drawButton(currentPage, button);
    button->debugSerial("clearFiles");

    // Get next button
    button = currentPage->nextButton();
  }
}

bool Menu::addFile(ElementFile *file)
{
  Serial.println("pre addFile 2"); delay(1000);
  thisMenu->currentPage->addButton(file);
}
*/

ElementPage *Menu::getCurrentPage()
{
  return thisMenu->currentPage;
}

void Menu::debugSerial(const char *debugLocation)
{
  Serial.print(" <");
  Serial.print(__FILENAME__);
  Serial.print("> ");

  Serial.print(F(" thisMenu '0x "));
  Serial.println((unsigned int)(thisMenu), HEX);

  Serial.print(F("' debug Loc: '"));
  Serial.print(debugLocation);
  Serial.print(F("' "));
}

#endif // Menu