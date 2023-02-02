#ifndef ElementRectangle_h
#define ElementRectangle_h

#include "Arduino.h"
#include "Dimensions.h"

#include "ElementBase.h"
#include "TBA_Macros.h"

class ElementRectangle : public ElementBase
{

private:
    Dimensions *dimensions;

protected:
public:
    ElementRectangle(const char * name, Dimensions *dimensions) : ElementBase(name)
    {
        this->dimensions = dimensions;
    }

    Dimensions *getDimensions()
    {
        return this->dimensions;
    }

    void debugSerial(const char * debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        this->dimensions->debugSerial("NO_CR");

        ElementBase::debugSerial(debugLocation);
    }
};

#endif
