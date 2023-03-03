/*
  PageFileSelectControl.h - Library for PageFileSelectControl code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageFileSelectControl_h
#define PageFileSelectControl_h

#include "Arduino.h"

#include "GlobalConst.h"
#include "Skin.h"

#include <sstream>
#include <string>

#include "Dimensions.h"
#include "ElementArg.h"
#include "ControlButton.h"
#include "ControlFile.h"
#include "ControlInput.h"
#include "ControlPage.h"
#include "ControlRectangle.h"

// #define FAKE_FILE_SYSTEM
// #define DEBUG
class PageFileSelectControl : public ControlPage
{
private:
  inline static const char *CURRENT_DIRECTORY = "currentDirectory";
  inline static uint8_t CURRENT_DIRECTORY_SIZE = 25;
  inline static const char *BACKDROP = "backDrop";
  inline static const char *SLIDER = "slider";
  inline static const char *SIDEBAR = "sideBar";

  inline static const char *UP = "U";
  inline static const char *DOWN = "D";

protected:
public:
  const char *parentDirectory;  // Folder as root for control
  std::string currentDirectory; // path to file, including sub(s) directory of the parentDirectory

  uint8_t pageNum;        // Current Page #: 0-x
  uint8_t pageSize;       // # files per page
  uint8_t filesLoadedCnt; // # files in directory

  PageFileSelectControl(const char *name, uint8_t backPageDelay, const char *backPage, const char *parentDirectory, boolean displayHeader = false)
      : ControlPage(name, 0, displayHeader, true, backPageDelay, backPage, NULL)
  {
    while (!backPage)
    { // loop forever.
      Serial.println(F("backPage cannot be NULL for PageFileSelectControl... Required for '..' exit."));
      delay(1000);
    }

    this->parentDirectory = parentDirectory;
    this->currentDirectory = this->parentDirectory;

    Dimensions *dimensions;

    uint8_t size = 25; // U & D button size;
    uint16_t top = 0;

    if (this->isVisible())
    {
      top = skin->getHeaderHeight();
    }

    // =============================================================================
    // Add input directory name
    dimensions = new Dimensions(skin->buttonMargin, top + skin->buttonMargin, skin->getScreenWidth() - (skin->buttonMargin * 1) - (skin->buttonBorderWidth * 2), CURRENT_DIRECTORY_SIZE);
    this->addInput(new ControlInput(CURRENT_DIRECTORY, CURRENT_DIRECTORY_SIZE, ControlInput::JUSTIFICATION::LEFT, dimensions, currentDirectory.c_str()));

    // =============================================================================
    // Add rectangles back drop
    top = dimensions->getY() + dimensions->getH() + skin->buttonMargin;
    dimensions = new Dimensions(skin->buttonMargin, top, skin->getScreenWidth() - size - (skin->buttonMargin * 2) - (skin->buttonBorderWidth * 2), skin->getScreenHeight() - top - (skin->buttonMargin * 1));
    this->addRectangle(new ControlRectangle(BACKDROP, dimensions));

    // =========================================================================
    // --------------------------- Scrole Bar ----------------------------------
    // Add button: UP
    dimensions = new Dimensions(skin->getScreenWidth() - size - skin->buttonMargin, top, size, size);
    this->addButton(new ControlButton(UP, dimensions, NULL));
    uint8_t scrollBarTop = dimensions->getY() + dimensions->getH();

    // Add button DW
    dimensions = new Dimensions(skin->getScreenWidth() - size - skin->buttonMargin, skin->getScreenHeight() - size - skin->buttonMargin, size, size);
    this->addButton(new ControlButton(DOWN, dimensions, NULL));
    uint8_t scrollBarHeight = skin->getScreenHeight() - scrollBarTop - size - skin->buttonMargin; // top + skin->buttonMargin;

    // Add rectangle sideBar bar (between U & D) - also used to cal slider
    dimensions = new Dimensions(skin->getScreenWidth() - size - skin->buttonMargin, scrollBarTop, size, scrollBarHeight);
    this->addRectangle(new ControlRectangle(SIDEBAR, dimensions));

    // Add rectangle slider (position box)
    dimensions = new Dimensions(skin->getScreenWidth() - size, top + size, size - 10, size);
    this->addRectangle(new ControlRectangle(SLIDER, dimensions));
  }

  // This method needs to be implemented by the users.
  virtual boolean isFileWorthy(File entry)
  {
    while (true)
    {
      Serial.println("You must create this 'virtual boolean isFileWorthy(File entry)' as it is required to work...");
      delay(1000);
    }
    return true; // all files valid...
  }

  void loadPageFiles()
  { // this method is called load, up, & down

    this->clearFiles(); // start with clean slate.

    uint8_t screenPos; // # files in directory
    filesLoadedCnt = 0;

    // Adjust backDrop is used to call file location on the screen.
    ControlRectangle *backDrop = getPageRectangle(BACKDROP);

    if (!backDrop)
    {
      Serial.println("Rect not found: 'backDrop'");
      return;
    }

    uint16_t top = backDrop->getDimensions()->getH();
    uint16_t height = 19;
    uint16_t bottom = 10;

    pageSize = ((top - bottom) / height);

    if (pageNum == 0) // if first page, add ".." for exit control
    {
      Dimensions *dimensions = new Dimensions(backDrop->getDimensions()->getX() + 6,
                                              backDrop->getDimensions()->getY() + (screenPos * height) + 3,
                                              backDrop->getDimensions()->getW() - 10,
                                              height);

      if (strcmp(this->currentDirectory.c_str(), parentDirectory) == 0)
        addFile(new ControlFile("..", dimensions, this->backPage, NULL, true));
      else
        addFile(new ControlFile("..", dimensions, NULL, NULL, true));

      screenPos++;
      filesLoadedCnt++;
    }

    ControlInput *findInput = getPageInput(CURRENT_DIRECTORY);
    if (findInput)
    {
      findInput->clear();
      findInput->append(this->currentDirectory.c_str());
    }
    else
    {
      Serial.print("Did not find required label: '");
      Serial.print(CURRENT_DIRECTORY);
      Serial.println("'");
    }

    // Add files to list
#ifndef FAKE_FILE_SYSTEM
    File dir = SD.open(this->currentDirectory.c_str());
    File entry = dir.openNextFile();
    while (entry)
    {
      if (isFileWorthy(entry))
      {
        if (filesLoadedCnt >= (pageNum * pageSize) && (filesLoadedCnt < ((pageNum + 1) * pageSize)))
        { // Add worthy file to show
          Dimensions *dimensions = new Dimensions(backDrop->getDimensions()->getX() + 6,
                                                  backDrop->getDimensions()->getY() + (screenPos * height) + 3,
                                                  backDrop->getDimensions()->getW() - 10,
                                                  height);
          screenPos++;

          if (entry.isDirectory())
            addFile(new ControlFile(entry.name(), dimensions, NULL, NULL, entry.isDirectory()));
          else
            addFile(new ControlFile(entry.name(), dimensions, "PageOK", NULL, entry.isDirectory()));
        }
        filesLoadedCnt++;
      }
      entry = dir.openNextFile();
    }
    entry.close();
    dir.close();
#else
    uint8_t simFiles = (rand() % 4) + 3; // min 3 & max 7 files and directories
    while (simFiles != 0)
    {
      if (filesLoadedCnt >= (pageNum * pageSize) && (filesLoadedCnt < ((pageNum + 1) * pageSize)))
      { // Add selected file to show
        Dimensions *dimensions = new Dimensions(backDrop->getDimensions()->getX(), backDrop->getDimensions()->getY() + (screenPos * height), backDrop->getDimensions()->getW(), height);
        screenPos++;

        if (rand() % 4)
          addFile(new ControlFile("file", dimensions, "PageOK", NULL, false));
        else
          addFile(new ControlFile("dir", dimensions, NULL, NULL, true));
      }
      filesLoadedCnt++;
      simFiles--;
    }
#endif

    ControlRectangle *sidebar = getPageRectangle(SIDEBAR);
    if (!sidebar)
    { // sideBar is what we place the slider on.
      Serial.println("Rectangle not found: 'sideBar'");
      return;
    }

    // Slider - calc % from top and % from bottom position (Indicate scroll position)
    uint8_t pageCount = round(((double)filesLoadedCnt / (double)pageSize) + 0.49999);
    double percentY = (double)pageNum / (double)pageCount;
    double percentH = 1.0 / (double)pageCount;
    uint8_t perY = (uint8_t)(sidebar->getDimensions()->getY() + (percentY * sidebar->getDimensions()->getH()) + 4);
    uint8_t perH = (uint8_t)((sidebar->getDimensions()->getH() * percentH) - 8);

    // Remove existing slider bar rectangle.
    if (!removePageRectangle(SLIDER))
    {
      Serial.println("Rect delete failed: 'slider'");
      return;
    }

    // Add new slider
    Dimensions *dimensions = new Dimensions(sidebar->getDimensions()->getX() + 4, perY, sidebar->getDimensions()->getW() - 8, perH);
    ControlRectangle *slider = new ControlRectangle(SLIDER, dimensions);
    if (!addRectangle(slider))
    {
      Serial.println("Rect add failed:  'slider'");
    }
  }

  void load()
  {
    this->currentDirectory.clear();
    this->currentDirectory = parentDirectory;
    this->pageNum = 0;
    this->filesLoadedCnt = 0;
    loadPageFiles();
  }

  void exit()
  {
    this->clearFiles();
  }

  boolean upButton()
  {
    if (pageNum > 0)
    {
      pageNum--;
      loadPageFiles();
      return true;
    }
    return false;
  }
  boolean downButton()
  {
    if (filesLoadedCnt > (pageNum + 1) * pageSize)
    {
      pageNum++;
      loadPageFiles();
      return true;
    }
    return false;
  }

  boolean appendCurrentDirectory(const char *name)
  {

    if (this->currentDirectory.size() > 1)
    {
      this->currentDirectory.append("/");
    }
    this->currentDirectory.append(name);

    return true;
  }
  boolean removeCurrentDirectory()
  {
    if (this->currentDirectory.size() > 0)
    {
      std::string temp;

      std::size_t found = this->currentDirectory.find_last_of("/");
      if (found > 0)
      {
        temp = this->currentDirectory.substr(0, found);
      }
      else
      {
        temp = this->currentDirectory.substr(0, found + 1);
      }

      this->currentDirectory.clear();
      this->currentDirectory = temp.c_str();

      return true;
    }
    return false;
  }
  boolean dirSelect(const char *name) // boolean return is for redraw screen
  {
    boolean redraw = false;
    if (strcmp(name, "..") == 0)
    {
      // move back a dir
      redraw = removeCurrentDirectory();
    }
    else
    {
      redraw = appendCurrentDirectory(name);
    }

    if (redraw)
    {
      this->pageNum = 0;
      loadPageFiles();
    }
    return redraw;
  }
  boolean fileSelect(const char *name) // boolean return is for redraw screen
  {
    ElementArg *argPath = new ElementArg((const char *)"pathSelected", currentDirectory.c_str());
    (PageFileSelectControl *)Menu::getInstance()->addArg(argPath);

    ElementArg *argFile = new ElementArg((const char *)"fileSelected", name);
    (PageFileSelectControl *)Menu::getInstance()->addArg(argFile);

    return false;
  }

  boolean buttonShortPress(ControlButton *button)
  {
    if (strcmp(UP, button->getName()) == 0)
      return upButton();
    else if (strcmp(DOWN, button->getName()) == 0)
      return downButton();

    if (button->isStyleFile())
    {
      ControlFile *file = (ControlFile *)button;
      if (file->isDirectory())
      {
        return dirSelect(button->getName());
      }
      else
      {
        return fileSelect(button->getName());
      }
    }
    else
    { // ControlFile not found
      Serial.print("buttonShortPress is NOT a ***FILE*** '");
      Serial.print(button->getName());
      Serial.print("' value '");
      Serial.print(button->getValue());
      Serial.println("'");
    }
    return false;
  }
};

#endif
