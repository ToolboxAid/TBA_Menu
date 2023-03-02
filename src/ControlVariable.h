#ifndef ControlVariable_h
#define ControlVariable_h

#include "Arduino.h"

#include "ControlBase.h"
#include "Point.h"

#include "TBA_Macros.h"

class ControlVariable : public ControlBase
{
private:
    Point *point;
    void *value;
    const char *format;
    char *lcdValue;

protected:
public:
    ControlVariable(const char *name, void *value, const char *format, char *lcdValue, Point *point) : ControlBase(name) //----->call base class
    {
        this->point = point;
        this->value = value;
        this->format = format;
        this->lcdValue = lcdValue;
    }

    ~ControlVariable()
    {
        delete point;
        if (value)
            delete value;
        if (format)
            delete format;
        if (lcdValue)
            delete lcdValue;
    }

    void draw()
    {
        tft.setTextColor(getLCD()->getSkin()->textColor, getLCD()->getSkin()->textBackgroundColor);
        tft.setTextSize(getLCD()->getSkin()->textFontSize);
        tft.setTextDatum(TL_DATUM);

        tft.setCursor(this->point->getX(), this->point->getY());
        tft.print(this->lcdValue);
    }

    void *getValue()
    {
        return this->value;
    }
    const char *getFormat()
    {
        return this->format;
    }
    char *getLcdValue()
    {
        return this->lcdValue;
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print(F(" value: '0x"));
        Serial.print((unsigned int)value, HEX);

        Serial.print(F("' format: '0x"));
        Serial.print((unsigned int)format, HEX);
        Serial.print(F("' format: '"));
        Serial.print(format);

        Serial.print(F("' lcdValue: '0x"));
        Serial.print((unsigned int)lcdValue, HEX);
        Serial.print(F("' lcdValue: '"));
        Serial.print(lcdValue);

        Serial.print(F("' "));

        ControlBase::debugSerial(debugLocation);
    }
};

#endif
