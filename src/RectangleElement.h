#ifndef RectangleElement_h
#define RectangleElement_h

#include "Arduino.h"
#include "Dimensions.h"

#include "VirtualBase.h"
#include "TBA_Macros.h"

class RectangleElement : public VirtualBase
{

private:
    Dimensions *dimensions;

protected:
public:
    RectangleElement(std::string name, Dimensions *dimensions) : VirtualBase(name)
    {
        this->dimensions = dimensions;
    }

    Dimensions *getDimensions()
    {
        return this->dimensions;
    }

    void debugSerial(std::string debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        this->dimensions->debugSerial("NO_CR");

        VirtualBase::debugSerial(debugLocation);
    }
};

#endif
