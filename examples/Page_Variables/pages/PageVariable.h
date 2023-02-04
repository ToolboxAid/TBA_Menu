/*
  PageVariable.h - Library for PageVariable code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageVariable_h
#define PageVariable_h

#include "Arduino.h"

#include "GlobalConst.h"

#include <sstream>
#include <string>

#include "PageBase.h"
#include "Point.h"
#include "ElementLabel.h"
#include "ElementButton.h"

#include "TBA_Macros.h"
/* Format and it's comsumption (does not include the buffer space)
Format	Type      bytes
======  ========  =====
  %c   	char      1/char
  %i  	int       4
  %u  	unsigned  4
  %f  	float     4
        double    8
  %s  	string    1/char + 1 for terminater

https://www.programiz.com/cpp-programming/library-function/cstdio/sprintf
https://en.cppreference.com/w/cpp/io/c/fprintf
*/

/* https://www.learncpp.com/cpp-tutorial/void-pointers/ */

/*  Use free for such strings if they are C-style strings created with malloc.

char* s = (char*) malloc( 80* sizeof(char));
...
free(s);
Use delete[] for such strings if they are created with new.

char* s = new char[80]();
...
delete[] s;
Use delete for such strings if they are std::string created dynamically.

 std::string&  str = new std::string(s);
...

delete str;
Don’t try to delete strings that are not dynamically allocated.

char * hw = "Hello World";
char gw[14] = "Goodbye World"; // NUL char at end = 14
std::string s{"Hello Goodbye Hello Goodbye"};
*/

/*  template<typename T> void query(const string &whattoask, T &input) {
  cout << whattoask << endl;
  cin >> input;
  cout << endl;
}

int main() {
  int i;
  double d;
  string s;

  query("An integer: ", i);
  query("Floating point: ", d);
  query("A word: ", s);
}*/

class PageVariable
{
private:
  inline static const char *chrVarName = "chrVar"; // Starting from C++17
  inline static const char *chrVarFormat = "%7c";

  inline static const char *intVarName = "intVar"; // Starting from C++17
  inline static const char *intVarFormat = "%7d";

  inline static const char *fltVarName = "fltVar";
  inline static const char *fltVarFormat = "%2i.%04i";

  inline static const char *dblVarName = "dblVar";
  inline static const char *dblVarFormat = "%3i.%03i";

  inline static const char *strVarName = "strVar"; // Starting from C++17
  inline static const char *strVarFormat = "%s";

  inline static const char *bolVarName = "bolVar"; // Starting from C++17
  inline static const char *bolVarFormat = "%s";

protected:
public:
  static void chrAdd(ElementPage *page)
  {
    // Add labels
    page->addLabel(new ElementLabel(chrVarName, new Point(75, 60)));

    // initialize chrVar = 100;
    char *chrVar = new char(' ');

    // allocate space for the chrDisplay, len of 7i + 1 terminator \0
    char *chrDisplay = new char[8]{'1', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element
    ElementVariable *elementVariable = new ElementVariable(chrVarName, (void *)chrVar, chrVarFormat, chrDisplay, new Point(165, 60));

    // Add it to the page
    page->addVariable(elementVariable);
  }
  static void chrUpdateExample()
  { // char - stores single characters, such as 'a' or 'B'. Char values are surrounded by single quotes
    ElementVariable *element = Menu::getInstance()->getCurrentPage()->getPageVariable(chrVarName);
    char *chrVarValue = (char *)element->getValue();
    *chrVarValue += 1;
    sprintf(element->getDisplay(), element->getFormat(), (char)*chrVarValue);
  }

  static void intAdd(ElementPage *page)
  {
    // Add labels
    page->addLabel(new ElementLabel(intVarName, new Point(75, 80)));
    // Add INT variable

    // initialize intVar = 100;
    int *intVar = new int(100);

    // allocate space for the intDisplay, len of 7i + 1 terminator \0
    char *intDisplay = new char[8]{'1', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element
    ElementVariable *elementVariable = new ElementVariable(intVarName, (void *)intVar, intVarFormat, intDisplay, new Point(165, 80));

    // Add it to the page
    page->addVariable(elementVariable);
  }
  static void intUpdateExample()
  { // int - stores integers (whole numbers), without decimals, such as 123 or -123
    ElementVariable *element = Menu::getInstance()->getCurrentPage()->getPageVariable(intVarName);
    int *intVarValue = (int *)element->getValue();
    *intVarValue += 1;
    // snprintf_P(element->getDisplay(), strlen(element->getDisplay()) + 1, element->getFormat(), *intVarValue);
    sprintf(element->getDisplay(), element->getFormat(), (int)*intVarValue);
  }

  static void fltAdd(ElementPage *page)
  {
    // Add labels
    page->addLabel(new ElementLabel(fltVarName, new Point(75, 100)));

    // Add Float variable  fltVar
    float *fltVar = new float(0.0);

    // allocate space for the fltDisplay, len of 7i + 1 terminator
    char *fltDisplay = new char[8]{'9', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element
    ElementVariable *elementVariable = new ElementVariable(fltVarName, (void *)fltVar, fltVarFormat, fltDisplay, new Point(165, 100));
    // Add it to the page
    page->addVariable(elementVariable);
  }
  static void fltUpdateExample()
  { // float - stores floating point numbers, with decimals, such as 19.99 or -19.99
    ElementVariable *element = Menu::getInstance()->getCurrentPage()->getPageVariable(fltVarName);
    float *fltVarValue = (float *)element->getValue();
    *fltVarValue += 0.0025;

    float fractpart, intpart;
    fractpart = modf((float)*fltVarValue, &intpart);

    sprintf(element->getDisplay(), element->getFormat(), (int)intpart, (int)(fractpart * 10000));
  }

  static void dblAdd(ElementPage *page)
  {
    // Add labels
    page->addLabel(new ElementLabel(dblVarName, new Point(75, 120)));

    // Add Double variable  dblVar
    double *dblVar = new double(0.0);

    // allocate space for the dblDisplay, len of 7i + 1 terminator
    char *dblDisplay = new char[8]{'8', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element
    ElementVariable *elementVariable = new ElementVariable(dblVarName, (void *)dblVar, dblVarFormat, dblDisplay, new Point(165, 120));

    // Add it to the page
    page->addVariable(elementVariable);
  }
  static void dblUpdateExample()
  { // double - stores floating point numbers, with decimals, such as 19.99 or -19.99
    ElementVariable *element = Menu::getInstance()->getCurrentPage()->getPageVariable(dblVarName);
    double *dblVarValue = (double *)element->getValue();
    *dblVarValue += 1.111;

    double fractpart, intpart;
    fractpart = modf((double)*dblVarValue, &intpart);
    // snprintf_P(element->getDisplay(), strlen(element->getDisplay()) , element->getFormat(), (int)intpart, (int)(fractpart * 1000));
    sprintf(element->getDisplay(), element->getFormat(), (int)intpart, (int)(fractpart * 1000));
  }

  static void strAdd(ElementPage *page)
  {
    // Add labels
    page->addLabel(new ElementLabel(strVarName, new Point(75, 140)));
    // Add String variable  strVar

    double *strVar = new double(0.0);
    // allocate space for the strDisplay, len of 7i + 1 terminator
    char *strDisplay = new char[8]{'S', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element
    ElementVariable *elementVariable = new ElementVariable(strVarName, (void *)strVar, strVarFormat, strDisplay, new Point(165, 140));

    // Add it to the page
    page->addVariable(elementVariable);
  }
  static void strUpdateExample() // Update String Variable Example
  {                              // string - stores text, such as "Hello", "World". String values are surrounded by double quotes
    ElementVariable *element = Menu::getInstance()->getCurrentPage()->getPageVariable(strVarName);
    //                                   1         2
    //                         012345678901234567890123456/   0-26 = 27 char/bytes
    const char *aLongStrVar = "This is a very long string.";

    const char *src = aLongStrVar;
    size_t sizeS = strlen(aLongStrVar); // 27

    static uint8_t offset = 0;

    const char *offsetData = &(*(src + offset));
    size_t srcSize = strlen(offsetData); // 27 to 7

    char *dest = element->getDisplay();
    size_t destSize = strlen(dest); // 7

    if (srcSize <= destSize)
      offset = 0;
    else
      offset++;

    memcpy(element->getDisplay(), offsetData, destSize);
  }

  static void bolAdd(ElementPage *page)
  {
    // Add labels
    page->addLabel(new ElementLabel(bolVarName, new Point(75, 160)));

    // Add Boolean variable  bolVar
    boolean *bolVar = new boolean(0.0);
    // allocate space for the bolDisplay, len of 7i + 1 terminator
    char *bolDisplay = new char[8]{'T', '2', '3', '4', '5', '6', '7', '\0'};

    // create the variable element
    ElementVariable *elementVariable = new ElementVariable(bolVarName, (void *)bolVar, bolVarFormat, bolDisplay, new Point(165, 160));

    // Add it to the page
    page->addVariable(elementVariable);
  }
  static void bolUpdateExample()
  { // bool - stores values with two states: true or false
    ElementVariable *element = Menu::getInstance()->getCurrentPage()->getPageVariable(bolVarName);
    boolean *bolVarValue = (boolean *)element->getValue();
    sprintf(element->getDisplay(), element->getFormat(), formatBool(*bolVarValue));
    *bolVarValue = (rand() % 2);
  }

  static void buttonAdd(ElementPage *page)
  {
    // Add button 1
    // Dimensions *dimensions = new Dimensions(120, 190, 60, 25);
    ElementButton *button = new ElementButton("Main",
                                              /* X, Y, W, H             */ new Dimensions(120, 200, 60, 25),
                                              /* short press            */ NULL,
                                              /* new short current page */ "Main",
                                              /* long  press            */ NULL,
                                              /* new long current page  */ NULL);
    page->addButton(button);
  }

  static ElementPage *create()
  {
    ElementPage *page = new ElementPage("Vars Example",
                                        /* static func to exec  */ &PageVariable::pageRefresh,
                                        /* ms refrest seconds   */ 1,
                                        /* display Header       */ true,
                                        /* clear screen         */ true,
                                        /* backPageDelay seconds*/ 200,
                                        /* backPage             */ "Main");

    chrAdd(page);
    intAdd(page);
    fltAdd(page);
    dblAdd(page);
    strAdd(page);
    bolAdd(page);

    buttonAdd(page);

    /* Don't do this, errors on display.
    delete dimensions;
    delete button;
    */

    return page;
  }

  static void pageRefresh()
  { // Do some work on a specific variables on the page
    // Serial.println("pageRefresh");

    chrUpdateExample();
    intUpdateExample();
    fltUpdateExample();
    dblUpdateExample();
    strUpdateExample();
    bolUpdateExample();
  }
};

#endif
