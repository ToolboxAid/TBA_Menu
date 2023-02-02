#ifndef ElementLabel_h
#define ElementLabel_h

#include "Arduino.h"
#include "Point.h"

#include "ElementBase.h"
#include "TBA_Macros.h"

class ElementLabel : public ElementBase
{

private:
    Point *point;
    bool drawBackgroundColor;

protected:
public:
    ElementLabel(const char * name, Point *point, bool drawBackgroundColor = true) : ElementBase(name)
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

    void debugSerial(const char * debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        this->point->debugSerial("4Label");

        ElementBase::debugSerial(debugLocation);
    }
};

#endif
