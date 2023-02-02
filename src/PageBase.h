/*
  PageBase.h - Library for PageBase code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageBase_h
#define PageBase_h

#include "Arduino.h"

// #include "GlobalConst.h"

// // ConvertString
// // #include <iostream>
// #include <sstream>
// #include <string>

class PageBase
{
private:
protected:
  PageBase()
  {
  }

public:

template <typename T>
static T ConvertChar(const char *nameIn, const char *dataIn, bool &success)
{
  success = true;
  std::string data(dataIn);

  //return ConvertString(*nameIn, *data, *success);

  if(!data.empty())
  {
    T ret;
    std::istringstream iss(data);
    if(data.find("0x") != std::string::npos)
    {
      iss >> std::hex >> ret;
    }
    else
    {
      iss >> std::dec >> ret;
    }

    if(iss.fail())
    {
      success = false;
        Serial.print  ("conversion failed for varName: '");
        Serial.print  (nameIn);
        Serial.print  ("' value: '");
        Serial.print  (dataIn);
        Serial.println("'");
      return T();
    }
    return ret;
  }
  return T();

}

};

#endif
