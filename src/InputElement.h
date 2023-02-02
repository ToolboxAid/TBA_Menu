#ifndef InputElement_h
#define InputElement_h

#include "Arduino.h"
//#include "Point.h"
#include "Dimensions.h"

#include "VirtualBase.h"
#include "TBA_Macros.h"

class InputElement : public VirtualBase
{
private:
    Dimensions *dimensions;
    std::string input;

protected:
public:
    InputElement(std::string name, std::string input, Dimensions *dimensions, uint16_t externalID = 0)
        : VirtualBase(name, externalID) //----->call base class
    {
        this->dimensions = dimensions;
        this->input = input;
    }

    Dimensions *getDimensions()
    {
        return this->dimensions;
    }
    std::string getInput()
    {
        return this->input;
    }
    void setInput(std::string input)
    {
        this->input = input.c_str();
    }

    void debugSerial(std::string debugLocation)
    {
        Serial.print  (F(" <"));
        Serial.print  (__FILENAME__);
        Serial.print  (F("> "));

        Serial.print  (F(" input: "));
        Serial.print  (input.c_str());

        VirtualBase::debugSerial(debugLocation);
    }
};

#endif
