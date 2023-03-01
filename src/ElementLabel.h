#ifndef ElementLabel_h
#define ElementLabel_h

#include "Arduino.h"
#include "Point.h"

#include "ElementBase.h"
#include "TBA_Macros.h"

#include "GlobalConst.h"
class ElementLabel : public ElementBase
{

private:
    Point *point;
    boolean drawBackgroundColor;

protected:
public:
    ElementLabel(const char *name, Point *point, boolean drawBackgroundColor = true) : ElementBase(name)
    {
        this->point = point;
        this->drawBackgroundColor = drawBackgroundColor;
    }

    ~ElementLabel()
    {
        if (this->point)
        {
            free(this->point);
        }
    }

    void draw()
    {
        LCD *lcd = LCD::GetInstance();

        tft.setTextSize(lcd->getSkin()->textFontSize);
        tft.setTextDatum(TL_DATUM);

        if (this->drawBackgroundColor)
        {
            tft.setTextColor(lcd->getSkin()->textColor, lcd->getSkin()->textBackgroundColor);
        }
        else
        {
            tft.setTextColor(lcd->getSkin()->textColor);
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

        ElementBase::debugSerial(debugLocation);
    }
};

#endif
