/*
  PageFileSelectControlWithHeader.h - Library for PageFileSelectControlWithHeader code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageFileSelectControlWithHeader_h
#define PageFileSelectControlWithHeader_h

#include "Arduino.h"

#include "Skin.h"

#include "PageFileSelectControl.h"

class PageFileSelectControlWithHeader : public PageFileSelectControl
{
private:
  inline static const char *NAME = "w/ Header";

  inline static const char *extensionIncludeArray[] = {".jpg", ".png", NULL};
  inline static const char *fileExcludeArray[] = {"System Volume Information", NULL};

protected:
public:
  PageFileSelectControlWithHeader() : PageFileSelectControl(NAME, 250, PageMain::NAME, "/", true)
  {
  }

  boolean isFileWorthy(File entry)
  {
    uint8_t excludeIndex = 0;
    while (fileExcludeArray[excludeIndex] != 0)
    {
      if (strcmp(entry.name(), fileExcludeArray[excludeIndex]) != 0)
      {
        if (entry.isDirectory())
        { // all directories are added
          return true;
        }
        else
        { // If ends with
          uint8_t includeIndex = 0;
          while (extensionIncludeArray[includeIndex] != 0)
          {
            int nameLen = strlen(entry.name());
            int extLen = strlen(extensionIncludeArray[includeIndex]);
            if (nameLen >= extLen && strcmp(entry.name() + nameLen - extLen, extensionIncludeArray[includeIndex]) == 0)
            {
              return true;
            }
            includeIndex++;
          }
        }
      }
      excludeIndex++;
    }
    return false;
  }

  void load()
  {
    PageFileSelectControl::load(); // if you wish to use this load() method, you must leave the call to parent load()
  }

  void exit()
  {
    PageFileSelectControl::exit(); // if you wish to use this exit() method, you must leave the call to parent exit()
  }
};

#endif
