/*
    This Button Element class provides the ability to:
    1) execute static funtion for short press
    2) Change current Page for short press
    3) execute a different static function when long press
    4) Change to a different current Page when long press
*/

#ifndef FileElement_h
#define FileElement_h

#include "Arduino.h"

#include "GlobalConst.h"

#include "Point.h"
#include "Dimensions.h"

#include "VirtualBase.h"
#include "ButtonElement.h"

#include "TBA_Macros.h"

class FileElement : public ButtonElement
{
public:
    // Use this to create a Button of style : FILE
    FileElement(std::string name, Dimensions *dimensions,
                void (*pShortFunction)(std::string), std::string shortPage,
                // std::string rootDirectory,
                std::string value, std::string icon) 
                : ButtonElement(name, dimensions, pShortFunction, shortPage, NULL, EMPTY, icon, value, false)
    {
        this->rootDirectory = rootDirectory;
        this->style = STYLE::FILE;
    }

    ~FileElement()
    {
        Serial.println("~FileElement()");

        rootDirectory.erase();
        
    }

    std::string getRootDirectory()
    {
        this->rootDirectory;
    }

    void debugSerial(std::string debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print(F(" root Dir: '"));
        Serial.print(this->rootDirectory.c_str());

        Serial.print(F("' "));

        ButtonElement::debugSerial(debugLocation);
    }



protected:
private:
    std::string rootDirectory;
};

#endif
