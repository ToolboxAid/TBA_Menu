#ifndef ControlLabel_h
#define ControlLabel_h

#include "Arduino.h"
#include "Point.h"

#include "ControlBase.h"
#include "TBA_Macros.h"

class ControlLabel : public ControlBase
{

private:
    Point *point;
    boolean drawBackgroundColor;
    char *value;

protected:
public:
    ControlLabel(const char *name, Point *point, boolean drawBackgroundColor = true)
        : ControlLabel(name, point, drawBackgroundColor, NULL)
    {
    }

    ControlLabel(const char *name, Point *point, boolean drawBackgroundColor, const char *value) : ControlBase(name)
    {
        this->point = point;
        this->drawBackgroundColor = drawBackgroundColor;

        if (value)
        {
            this->value = (char *)malloc(strlen(value) + 1);
            memcpy(this->value, value, strlen(value) + 1);
        }
        else
        {
            this->value = NULL;
        }
    }

    ~ControlLabel()
    {
        if (this->point)
            free(this->point);

        if (this->value)
            free(this->value);
    }

    void draw()
    {
        tft.setTextSize(skin->textFontSize);
        tft.setTextDatum(TL_DATUM);

        if (this->drawBackgroundColor)
        {
            tft.setTextColor(skin->textColor, skin->textBackgroundColor);
        }
        else
        {
            tft.setTextColor(skin->textColor);
        }

        tft.setCursor(this->point->getX(), this->point->getY());
        if (this->value)
            tft.print(this->value);
        else
            tft.print(this->getName());
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print(F(" value: '"));
        Serial.print(this->value);
        Serial.print(F("' "));

        this->point->debugSerial("NO_CR");

        ControlBase::debugSerial(debugLocation);
    }
};

#endif
