#ifndef ElementLabel_h
#define ElementLabel_h

#include "Arduino.h"
#include "Point.h"

#include "ControlBase.h"
#include "TBA_Macros.h"

#include "GlobalTFT.h"
class ElementLabel : public ControlBase
{

private:
    Point *point;
    boolean drawBackgroundColor;

protected:
public:
    ElementLabel(const char *name, Point *point, boolean drawBackgroundColor = true) : ControlBase(name)
    {
        this->point = point;
        this->drawBackgroundColor = drawBackgroundColor;
    }

    ~ElementLabel()
    {
        if (this->point)
            free(this->point);
    }

    void draw()
    {
        tft.setTextSize(getLCD()->getSkin()->textFontSize);
        tft.setTextDatum(TL_DATUM);

        if (this->drawBackgroundColor)
        {
            tft.setTextColor(getLCD()->getSkin()->textColor, getLCD()->getSkin()->textBackgroundColor);
        }
        else
        {
            tft.setTextColor(getLCD()->getSkin()->textColor);
        }
        tft.setCursor(this->point->getX(), this->point->getY());
        tft.print(this->getName());
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        this->point->debugSerial("NO_CR");

        ControlBase::debugSerial(debugLocation);
    }
};

#endif
