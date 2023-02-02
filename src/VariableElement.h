#ifndef VariableElement_h
#define VariableElement_h

#include "Arduino.h"
#include "Point.h"

#include "VirtualBase.h"
#include "TBA_Macros.h"

class VariableElement : public VirtualBase
{
private:
    Point *point;
    std::string value;

protected:
public:
    VariableElement(std::string name, std::string value, Point *point, uint16_t externalID = 0)
        : VirtualBase(name, externalID) //----->call base class
    {
        this->point = point;
        this->value = value;
    }

    Point *getPoint()
    {
        return this->point;
    }
    std::string getValue()
    {
        return this->value;
    }
    void setValue(std::string value)
    {
        this->value = value.c_str();
    }

    void debugSerial(std::string debugLocation)
    {
        Serial.print  (F(" <"));
        Serial.print  (__FILENAME__);
        Serial.print  (F("> "));

        Serial.print  (F(" value: "));
        Serial.print  (value.c_str());

        VirtualBase::debugSerial(debugLocation);
    }
};

#endif
