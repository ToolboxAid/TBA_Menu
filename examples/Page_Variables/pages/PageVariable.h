/*
  PageVariable.h - Library for PageVariable code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageVariable_h
#define PageVariable_h

#include "Arduino.h"

#include "Point.h"
#include "Dimensions.h"

#include "ControlButton.h"
#include "ControlLabel.h"
#include "ControlVariable.h"

#include "TBA_Macros.h"

class PageVariable : public ControlPage
{
private:
  inline static const char *NAME = "Vars Example";
  inline static const char *MAIN = "Main";

  inline static const char *chrVarName = "chrVar";
  inline static const char *chrVarFormat = "%7c";

  inline static const char *intVarName = "intVar";
  inline static const char *intVarFormat = "%7d";

  inline static const char *fltVarName = "fltVar";
  inline static const char *fltVarFormat = "%2i.%04i";

  inline static const char *dblVarName = "dblVar";
  inline static const char *dblVarFormat = "%3i.%03i";

  inline static const char *strVarName = "strVar";
  inline static const char *strVarFormat = "%s";
  inline static const char *aLongStrVar = "This is a very long string variable to display for a scroll example.";

  inline static const char *bolVarName = "bolVar";
  inline static const char *bolVarFormat = "%s";

protected:
public:
  void chrAdd()
  {
    // Add labels
    this->addLabel(new ControlLabel(chrVarName, new Point(75, 60)));

    // initialize chrVar = 100;
    char *chrVar = new char(' ');

    // allocate space for the chrLcdValue, len of 7i + 1 terminator \0
    char *chrLcdValue = new char[8]{'1', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element add it to the page
    this->addVariable(new ControlVariable(chrVarName, (void *)chrVar, chrVarFormat, chrLcdValue, new Point(165, 60)));
  }
  void chrRefreshExample()
  { // char - stores single characters, such as 'a' or 'B'. Char values are surrounded by single quotes
    ControlVariable *element = getPageVariable(chrVarName);
    char *chrVarValue = (char *)element->getValue();
    *chrVarValue += 1;
    sprintf(element->getLcdValue(), element->getFormat(), (char)*chrVarValue);
  }

  void intAdd()
  {
    // Add labels
    this->addLabel(new ControlLabel(intVarName, new Point(75, 80)));
    // Add INT variable

    // initialize intVar = 100;
    int *intVar = new int(100);

    // allocate space for the intLcdValue, len of 7i + 1 terminator \0
    char *intLcdValue = new char[8]{'1', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element add it to the page
    this->addVariable(new ControlVariable(intVarName, (void *)intVar, intVarFormat, intLcdValue, new Point(165, 80)));
  }
  void intRefreshExample()
  { // int - stores integers (whole numbers), without decimals, such as 123 or -123
    ControlVariable *element = getPageVariable(intVarName);
    int *intVarValue = (int *)element->getValue();
    *intVarValue += 1;
    // snprintf_P(element->getLcdValue(), strlen(element->getLcdValue()) + 1, element->getFormat(), *intVarValue);
    sprintf(element->getLcdValue(), element->getFormat(), (int)*intVarValue);
  }

  void fltAdd()
  {
    // Add labels
    this->addLabel(new ControlLabel(fltVarName, new Point(75, 100)));

    // Add Float variable  fltVar
    float *fltVar = new float(0.0);

    // allocate space for the fltLcdValue, len of 7i + 1 terminator
    char *fltLcdValue = new char[8]{'9', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element add it to the page
    this->addVariable(new ControlVariable(fltVarName, (void *)fltVar, fltVarFormat, fltLcdValue, new Point(165, 100)));
  }
  void fltRefreshExample()
  { // float - stores floating point numbers, with decimals, such as 19.99 or -19.99
    ControlVariable *element = getPageVariable(fltVarName);
    float *fltVarValue = (float *)element->getValue();
    *fltVarValue += 0.0025;

    float fractpart, intpart;
    fractpart = modf((float)*fltVarValue, &intpart);

    sprintf(element->getLcdValue(), element->getFormat(), (int)intpart, (int)(fractpart * 10000));
  }

  void dblAdd()
  {
    // Add labels
    this->addLabel(new ControlLabel(dblVarName, new Point(75, 120)));

    // Add Double variable  dblVar
    double *dblVar = new double(0.0);

    // allocate space for the dblLcdValue, len of 7i + 1 terminator
    char *dblLcdValue = new char[8]{'8', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element add it to the page
    this->addVariable(new ControlVariable(dblVarName, (void *)dblVar, dblVarFormat, dblLcdValue, new Point(165, 120)));
  }
  void dblRefreshExample()
  { // double - stores floating point numbers, with decimals, such as 19.99 or -19.99
    ControlVariable *element = getPageVariable(dblVarName);
    double *dblVarValue = (double *)element->getValue();
    *dblVarValue += 1.111;

    double fractpart, intpart;
    fractpart = modf((double)*dblVarValue, &intpart);
    // snprintf_P(element->getLcdValue(), strlen(element->getLcdValue()) , element->getFormat(), (int)intpart, (int)(fractpart * 1000));
    sprintf(element->getLcdValue(), element->getFormat(), (int)intpart, (int)(fractpart * 1000));
  }

  void strAdd()
  {
    // Add labels
    this->addLabel(new ControlLabel(strVarName, new Point(75, 140)));
    // Add String variable  strVar

    double *strVar = new double(0.0);
    // allocate space for the strLcdValue, len of 7i + 1 terminator
    char *strLcdValue = new char[8]{'S', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element add it to the page
    this->addVariable(new ControlVariable(strVarName, (void *)strVar, strVarFormat, strLcdValue, new Point(165, 140)));
  }
  void strRefreshExample() // Update String Variable Example
  {                        // string - stores text, such as "Hello", "World". String values are surrounded by double quotes
    ControlVariable *element = getPageVariable(strVarName);
    //                                   1         2
    //                         012345678901234567890123456/   0-26 = 27 char/bytes

    const char *src = aLongStrVar;
    size_t sizeS = strlen(aLongStrVar); // 27

    static uint8_t offset = 0;

    const char *offsetData = &(*(src + offset));
    size_t srcSize = strlen(offsetData); // 27 to 7

    char *dest = element->getLcdValue();
    size_t destSize = strlen(dest); // 7

    if (srcSize <= destSize)
      offset = 0;
    else
      offset++;

    memcpy(element->getLcdValue(), offsetData, destSize);
  }

  void bolAdd()
  {
    // Add labels
    this->addLabel(new ControlLabel(bolVarName, new Point(75, 160)));

    // Add Boolean variable  bolVar
    boolean *bolVar = new boolean(0.0);
    // allocate space for the bolLcdValue, len of 7i + 1 terminator
    char *bolLcdValue = new char[8]{'T', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element add it to the page
    this->addVariable(new ControlVariable(bolVarName, (void *)bolVar, bolVarFormat, bolLcdValue, new Point(165, 160)));
  }
  void bolRefreshExample()
  { // boolean - stores values with two states: true or false
    ControlVariable *element = getPageVariable(bolVarName);
    boolean *bolVarValue = (boolean *)element->getValue();
    sprintf(element->getLcdValue(), element->getFormat(), formatBool(*bolVarValue));
    *bolVarValue = (rand() % 2);
  }

  PageVariable() : ControlPage(PageVariable::NAME, 1, true, true, 200, PageVariable::MAIN, NULL)
  {
    chrAdd();
    intAdd();
    fltAdd();
    dblAdd();
    strAdd();
    bolAdd();

    this->addButton(new ControlButton(PageVariable::MAIN, new Dimensions(120, 200, 60, 25), PageVariable::MAIN));
  }

  void refresh()
  {
    chrRefreshExample();
    intRefreshExample();
    fltRefreshExample();
    dblRefreshExample();
    strRefreshExample();
    bolRefreshExample();
  }
};

#endif
