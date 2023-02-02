// /*
//     This Button Element class provides the ability to:
//     1) execute static funtion for short press
//     2) Change current Page for short press
//     3) execute a different static function when long press
//     4) Change to a different current Page when long press
// */

// #ifndef ElementFile_h
// #define ElementFile_h

// #include "Arduino.h"

// #include "GlobalConst.h"

// #include "Point.h"
// #include "Dimensions.h"


// #include "ElementButton.h"

// #include "TBA_Macros.h"

// class ElementFile : public ElementButton
// {
// public:
//     // Use this to create a Button of style : FILE
//     ElementFile(const char * name, Dimensions *dimensions,
//                 void (*pShortFunction)(const char *), const char * shortPage,
//                 // const char * rootDirectory,
//                 const char * value, const char * icon) 
//                 : ElementButton(name, dimensions, pShortFunction, shortPage, NULL, NULL, icon, value, false)
//     {
//         this->rootDirectory = rootDirectory;
//         this->style = STYLE::FILE;
//     }

//     ~ElementFile()
//     {
//         Serial.println("~ElementFile()");

//         rootDirectory = NULL; // = NULL;;
        
//     }

//     const char * getRootDirectory()
//     {
//         this->rootDirectory;
//     }

//     void debugSerial(const char * debugLocation)
//     {
//         Serial.print(F(" <"));
//         Serial.print(__FILENAME__);
//         Serial.print(F("> "));

//         Serial.print(F(" root Dir: '"));
//         Serial.print(this->rootDirectory);

//         Serial.print(F("' "));

//         ElementButton::debugSerial(debugLocation);
//     }



// protected:
// private:
//     const char * rootDirectory;
// };

// #endif
