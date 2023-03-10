/*
  Menu.h - Library for Menu code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef Menu_h
#define Menu_h

#include "Arduino.h"

/* The SPIFFS (FLASH file system) is used
   to hold touch screen calibration data
*/

#include "FS.h"
#include <SPI.h>

#include "Point.h"
#include "Skin.h"

#include "ElementBase.h"

#include "ControlBase.h"
#include "ControlPage.h"
#include "ElementArg.h"
#include "ControlButton.h"
#include "ControlRectangle.h"
#include "LinkListPlus.h"

#include "PageTBA.h"

// #define DEBUG
#define SCREEN_CAPTURE true

/* This is a singleton class,
only one instance will ever be created
*/
class Menu
{
private:
  static Menu *thisMenu;
  Menu() {}

  void traversMenuLists();
  boolean hasPageChange();
  boolean hasPageBack();

  void validPages(const char *, const char *);

  LinkListPlus *pageListPlus = new LinkListPlus();
  LinkListPlus *argsListPlus = new LinkListPlus();

  unsigned long pageLoadTime = 0;
  ControlPage *currentPage = NULL;
  ControlPage *newPage = NULL;

  // Store the touch coordinates
  uint16_t t_x = 0, t_y = 0;
  Point *point;

  boolean captureInProgress = false;

protected:
public:
  static Menu *getInstance()
  {
    if (!thisMenu)
    {
      thisMenu = new Menu();
    }
    return thisMenu;
  }

  void Initialize(Skin *skin, const char *lcd, const char *frontPage);

  uint16_t addPage(ControlPage *page);

  void checkMenuActions();

  ControlVariable *getPageVariable(const char *name);

  ControlInput *getPageInput(const char *name);

  ControlRectangle *getPageRectangle(const char *name);
  boolean removePageRectangle(const char *name);

  void clearArgs();
  uint16_t addArg(ElementArg *);
  ElementArg *searchArg(const char *);

  ControlPage *getCurrentPage();
  void setNewPage(const char *newPage);

  void debugSerial(const char *debugLocation);
};

/* Allocating and initializing GlobalClass's
   static data member.  The pointer is being
   allocated - not the object inself. */
Menu *Menu::thisMenu = NULL;

void Menu::Initialize(Skin *skin, const char *lcdName = "ILI9341", const char *frontPage = "Main")
{
  if (!skin)
    skin = new Skin(); // Use default skin

  // Init LCD & control
  LCD::Initialize(lcdName, skin->getRotate());
  ControlBase::setSkin(skin);

  // Add TBA Page & Set to current
  PageTBA *tbaPage = new PageTBA(frontPage);
  addPage((ControlPage *)tbaPage);
  this->newPage = tbaPage;
  this->currentPage = NULL;
}

uint16_t Menu::addPage(ControlPage *page)
{
  return this->pageListPlus->insertAtEnd(((ElementBase *)page));
}

void Menu::traversMenuLists()
{
#ifdef DEBUG
  Serial.println("=================================================");
  Serial.println("=================================================");
  Serial.println("=================================================");
  Serial.println("-------------------Current Page------------------");
  Serial.println(currentPage->getName());

  // Menu owned items
  Serial.println("- - - - - - - - - -ControlPage- - - - - - - - - -");
  pageListPlus->setCurrentToHead();
  ControlPage *page = (ControlPage *)pageListPlus->getNext();
  while (page)
  {
    page->debugSerial("traversLists - 'ControlPage'");
    page = (ControlPage *)pageListPlus->getNext();
  }

  Serial.println("- - - - - - - - - -ElementArg- - - - - - - - - -");
  argsListPlus->setCurrentToHead();
  ElementArg *arg = (ElementArg *)argsListPlus->getNext();
  while (arg)
  {
    arg->debugSerial("traversLists - 'ElementArg'");
    arg = (ElementArg *)argsListPlus->getNext();
  }

  // Page owned items
  currentPage->traverPageLists();
#endif
}

boolean Menu::hasPageChange()
{
  if (!captureInProgress)
    if (this->newPage != this->currentPage)
    {
      LCD *lcd = LCD::GetInstance();

      lcd->dumpFS(SD, "SD menu.h", "/", 0);
      lcd->dumpFS(SPIFFS, "SPIFFS menu.h", "/", 0);

      if (this->currentPage)
      {
        this->currentPage->exit();
      }
      this->currentPage = this->newPage;
      this->currentPage->load();
      this->currentPage->setInputs();
      // ------------------ TODO: remove this    this->traversMenuLists();

      this->pageLoadTime = millis();
      this->clearArgs();
      this->currentPage->drawPage(this->currentPage->getSkin()->textBackgroundColor); // skin->textBackgroundColor
      return true;
    }
  return false;
}

boolean Menu::hasPageBack()
{
  if (!captureInProgress)
    if (this->currentPage->hasBackPage())
    {
      if (millis() > (this->pageLoadTime + (this->currentPage->getBackPageDelay() * 1000)))
      {
        ControlPage *findPage = (ControlPage *)this->pageListPlus->searchName(this->currentPage->getBackPage());
        if (findPage)
        {
          this->newPage = findPage;
          return true;
        }
        else
        {
          this->pageLoadTime = millis(); // set time to prevent spam
          this->validPages("checkPageBack change not found: ", this->currentPage->getBackPage());
        }
      }
    }
  return false;
}

void Menu::validPages(const char *message, const char *findPage)
{
  Serial.print(message);
  Serial.print(" '");
  Serial.print(findPage);
  Serial.print("' backPageDelay: '");
  Serial.print(this->currentPage->getBackPageDelay());
  Serial.println("'");

  Serial.println("Valid Pages are: ");
  this->pageListPlus->setCurrentToHead();
  ControlPage *page = (ControlPage *)this->pageListPlus->getNext();
  while (page)
  {
    Serial.print("ID: '");
    Serial.print(page->getIdentity());
    Serial.print("' Name: '");
    Serial.print(page->getName());
    Serial.print("'");
    Serial.println();
    page = (ControlPage *)this->pageListPlus->getNext();
  }
  Serial.print("' Size: '");
  Serial.print(this->pageListPlus->getNodeCount());
  Serial.print("'");
  Serial.println();
}

void Menu::checkMenuActions()
{ // Check page actions (update screen based on current page, buttons, timer, event, ???)
  if (this->hasPageChange() || this->hasPageBack())
    return; // new page, not worth the risk of a delayed updates.

  LCD *lcd = LCD::GetInstance();

  boolean pressed;
  Point *point = lcd->getScreenTouchPoint(pressed);

  boolean redrawPage = false;

  this->currentPage->resetButtonIterator();
  ControlButton *button = this->currentPage->nextButton();
  while (button)
  {

    ControlButton::STATE triggerState = ControlButton::STATE::UP;

    if (button->hasStateChange(point, pressed, triggerState))
    {
      // Redraw button or file...
      if (button->isStyleButton())
        button->draw();
      else
        ((ControlFile *)button)->draw(); //???????????????????? Is this needed????

      if (button->isRELEASED())
      {
#ifdef DEBUG
        Serial.println("* * * * * * * * button->isRELEASED() * * * * * * * *");
        if (button->isStyleFile())
          ((ControlFile *)button)->debugSerial("ControlFile * * ");
        else
          button->debugSerial("ControlButton");
#endif
        button->resetButton();
        if (triggerState == ControlButton::STATE::SHORT)
        { // Short Press
          if (button->hasShortPage())
          {
            ControlPage *findPage = (ControlPage *)this->pageListPlus->searchName(button->getShortPage());
            if (findPage)
            {
              this->newPage = findPage;
            }
            else
            {
              this->pageLoadTime = millis(); // set time to prevent spam
              validPages("checkMenuActions short button change page not found: ", button->getShortPage());
              button->debugSerial("checkMenuActions 1");
            }
          }
          redrawPage = this->currentPage->buttonShortPress(button);
          break;
        }
        else
        { // Long Press
          if (button->hasLongPage())
          {
            ControlPage *findPage = (ControlPage *)this->pageListPlus->searchName(button->getLongPage());
            if (findPage)
            {
              this->newPage = findPage;
            }
            else
            {
              this->pageLoadTime = millis(); // set time to prevent spam
              validPages("checkMenuActions long button change page not found: ", button->getLongPage());
              button->debugSerial("checkMenuActions 2");
            }
          }
          redrawPage = this->currentPage->buttonLongPress(button);
          break;
        }
      }
      else if (button->isROLLOFF())
      { // User no longer on the button
        button->resetButton();
        break;
      }
    }

    button = this->currentPage->nextButton();
  }

  if (redrawPage)
  {
    traversMenuLists();
    this->currentPage->drawPage(this->currentPage->getSkin()->textBackgroundColor);
  }
  else if (this->currentPage->checkRefresh())
  {
    this->currentPage->drawVariables();
  }
  else
  {
    this->currentPage->drawInputs();
    if (SCREEN_CAPTURE)
      captureInProgress = lcd->screenCapture(this->currentPage->getName(), this->currentPage->getSkin()->getScreenWidth(), this->currentPage->getSkin()->getScreenHeight());
  }

  delete point; // delete anything we use NEW on.
}

ControlVariable *Menu::getPageVariable(const char *name)
{ // This is the only way to get access to current page
  return this->currentPage->getPageVariable(name);
}

ControlInput *Menu::getPageInput(const char *name)
{ // This is the only way to get access to current page
  return this->currentPage->getPageInput(name);
}

ControlRectangle *Menu::getPageRectangle(const char *name)
{ // This is the only way to get access to current page
  return this->currentPage->getPageRectangle(name);
}
boolean Menu::removePageRectangle(const char *name)
{
  return this->currentPage->removePageRectangle(name);
}

uint16_t Menu::addArg(ElementArg *arg)
{
  return this->argsListPlus->insertAtEnd((ElementBase *)arg);
}
ElementArg *Menu::searchArg(const char *name)
{
  return (ElementArg *)this->argsListPlus->searchName(name);
}
void Menu::clearArgs()
{
  // Setup iterator to top & loop
  this->argsListPlus->setCurrentToHead();
  ElementArg *arg = (ElementArg *)this->argsListPlus->getNext();
  while (arg)
  {
    // remove the arg
    if (!this->argsListPlus->deleteID(arg->getIdentity()))
    {
      arg->debugSerial("failed delete");
    }
    // Get next button
    arg = (ElementArg *)this->argsListPlus->getNext();
  }
}

ControlPage *Menu::getCurrentPage()
{
  return this->currentPage;
}

void Menu::setNewPage(const char *page)
{
  if (page)
  {
    this->newPage = (ControlPage *)pageListPlus->searchName(page);
    if (!this->newPage)
    {
      validPages("setNewPage not found: ", page);
    }
  }
}

void Menu::debugSerial(const char *debugLocation)
{
  Serial.print(" <");
  Serial.print(__FILENAME__);
  Serial.print("> ");

  Serial.print(F(" thisMenu '0x "));
  Serial.println((unsigned int)(this->thisMenu), HEX);

  Serial.print(F("' debug Loc: '"));
  Serial.print(debugLocation);
  Serial.print(F("' "));
}

#endif // Menu