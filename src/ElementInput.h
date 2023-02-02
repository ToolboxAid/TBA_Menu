#ifndef ElementInput_h
#define ElementInput_h

#include "Arduino.h"
#include "Dimensions.h"

#include "ElementBase.h"
#include "TBA_Macros.h"

class ElementInput : public ElementBase
{
private:
    Dimensions *dimensions;
    const char * input;

protected:
public:
    ElementInput(const char * name, const char * input, Dimensions *dimensions)
        : ElementBase(name) //----->call base class
    {
        this->dimensions = dimensions;
        this->input = input;
    }

    Dimensions *getDimensions()
    {
        return this->dimensions;
    }
    const char * getInput()
    {
        return this->input;
    }
    void setInput(const char * input)
    {
        this->input = input;
    }

    void debugSerial(const char * debugLocation)
    {
        Serial.print  (F(" <"));
        Serial.print  (__FILENAME__);
        Serial.print  (F("> "));

        Serial.print  (F(" input: "));
        Serial.print  (input);

        ElementBase::debugSerial(debugLocation);
    }
};

#endif
