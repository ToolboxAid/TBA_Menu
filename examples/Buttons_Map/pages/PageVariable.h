// /*
//   PageVariable.h - Library for PageVariable code.
//   Created by Mr Q, December 2, 2022.
//   Released into the public domain.
// */

// #pragma once

// #ifndef PageVariable_h
// #define PageVariable_h

// #include "Arduino.h"

// #include "GlobalConst.h"

// #include <sstream>
// #include <string>

// class PageVariable
// {
// private:
// protected:
// public:
//   static ElementPage * create()
//   {
//     ElementPage *page = new ElementPage("Vars Example",
//         /* static func to exec  */ &PageVariable::pageRefresh,
//         /* ms refrest seconds   */ 1,
//         /* display Header       */ true,
//         /* clear screen         */ true,
//         /* backPageDelay seconds*/ 20,
//         /* backPage             */ "Main");

//     // Add label
//     page->addLabel(*(new ElementLabel("Var1:", new Point(75, 80))));
//     page->addLabel(*(new ElementLabel("Var2:", new Point(75, 100))));
//     page->addLabel(*(new ElementLabel("Var3:", new Point(75, 120))));

//     // Add variable
//     page->addVariable(*(new ElementVariable("var1", "0", new Point(165, 80))));
//     page->addVariable(*(new ElementVariable("var2", "0", new Point(165, 100))));
//     page->addVariable(*(new ElementVariable("var3", "0.0", new Point(165, 120))));

//     // Add button 1
//     Dimensions *dimensions = new Dimensions(120, 190, 60, 25);
//     ElementButton *button = new ElementButton("Main",
//         /* X, Y, W, H             */ dimensions,
//         /* short press            */ NULL,
//         /* new short current page */ "Main",
//         /* long  press            */ NULL,
//         /* new long current page  */ NULL);
//     page->addButton(*button);

//     /* Don't do this, errors on display.
//     delete dimensions;
//     delete button;
//     */

//     return page;
//   }

//   /* Formating options...
//   Format	Type      bytes
//   ======  ========  =====
//   %c   	char      1/char
//   %i  	int       4
//   %u  	unsigned  4
//   %f  	float     4
//           double    8
//   %s  	string    1/char + 1 for terminater
//   */
//   static void pageRefresh()
//   { // Do some work on a specific variables page
//     // Serial.println("pageRefresh");
//     // Do some work on a specific variables page

//     const char * variableName;
//     const char * variableValue;

//     bool convert_success;
//     char buffer[10];
//     // ----------------------------------------------------------------------------------
//     // var1 of type unsigned short int
//     variableName = "var1";
//     variableValue = Menu::getInstance()->getPageVariable(variableName);
//     if (!variableValue  == NULL)
//     {
//       unsigned short int dNum = PageBase::ConvertString<unsigned short int>(variableName, variableValue, convert_success);

//       if (convert_success)
//       {
//         dNum += 256;

//         char buffer[10];

//         snprintf_P(buffer, sizeof(buffer), PSTR("%7i"), dNum);

//         const char * done = buffer;

//         bool oops = Menu::getInstance()->setPageVariable(variableName, done);
//         if (!oops)
//         {
//           // Serial.println("failed to save");
//           // already printed in getPageVariable.
//           // this is so you can do something if needed.
//         }
//       }
//     }

//     // ----------------------------------------------------------------------------------
//     // var2 of type int
//     variableName = "var2";
//     variableValue = Menu::getInstance()->getPageVariable(variableName);
//     if (!variableValue  == NULL)
//     {
//       int dNum = PageBase::ConvertString<int>(variableName, variableValue, convert_success);

//       if (convert_success)
//       {
//         dNum += 4096;

//         char buffer[10];

//         snprintf_P(buffer, sizeof(buffer), PSTR("%7i"), dNum);

//         const char * done = buffer;

//         bool oops = Menu::getInstance()->setPageVariable(variableName, done);
//         if (!oops)
//         {
//           // Serial.println("failed to save");
//           // already printed in getPageVariable.
//           // this is so you can do something if needed.
//         }
//       }
//     }

//     // ----------------------------------------------------------------------------------
//     // var3 of type double
//     variableName = "var3";
//     variableValue = Menu::getInstance()->getPageVariable(variableName);
//     if (!variableValue  == NULL)
//     {
//       double dNum = PageBase::ConvertString<double>(variableName, variableValue, convert_success);

//       if (convert_success)
//       {
//         dNum += 1.234;

//         double fractpart, intpart;
//         fractpart = modf(dNum, &intpart);

//         snprintf_P(buffer, sizeof(buffer), PSTR("%3i.%03i"), (int)intpart, (int)(fractpart * 1000));

//         const char * done = buffer;

//         bool oops = Menu::getInstance()->setPageVariable(variableName, done);
//         if (!oops)
//         {
//           // Serial.println("failed to save");
//           // already printed in getPageVariable.
//           // this is so you can do something if needed.
//         }
//       }
//     }
//   }

//   static void hiddenButtonShort()
//   { // Do some work on a specific variables page
//     // Serial.println("hiddenButtonShort");
//   }

//   static void hiddenButtonLong()
//   { // Do some work on a specific variables page
//     // Serial.println("hiddenButtonLong");
//   }
// };

// #endif
