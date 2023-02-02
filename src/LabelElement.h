#ifndef LabelElement_h
#define LabelElement_h

#include "Arduino.h"
#include "Point.h"

#include "VirtualBase.h"
#include "TBA_Macros.h"

class LabelElement : public VirtualBase
{

private:
    Point *point;
    bool drawBackgroundColor;

protected:
public:
    LabelElement(std::string name, Point *point, bool drawBackgroundColor = true) : VirtualBase(name)
    {
        this->point = point;
        this->drawBackgroundColor = drawBackgroundColor;
    }

    Point *getPoint()
    {
        return this->point;
    }
    bool getDrawBackgroundColor()
    {
        return this->drawBackgroundColor;
    }

    void debugSerial(std::string debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        this->point->debugSerial("4Label");

        VirtualBase::debugSerial(debugLocation);
    }
};

#endif
