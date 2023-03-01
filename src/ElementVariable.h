#ifndef ElementVariable_h
#define ElementVariable_h

#include "Arduino.h"

#include "ElementBase.h"
#include "Point.h"

#include "TBA_Macros.h"

#include "GlobalConst.h"
class ElementVariable : public ElementBase
{
private:
    Point *point;
    void *value;
    const char *format;
    char *lcdValue;

protected:
public:
    ElementVariable(const char *name, void *value, const char *format, char *lcdValue, Point *point) : ElementBase(name) //----->call base class
    {
        this->point = point;
        this->value = value;
        this->format = format;
        this->lcdValue = lcdValue;
    }

    ~ElementVariable()
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
        LCD *lcd = LCD::GetInstance();

        tft.setTextColor(lcd->getSkin()->textColor, lcd->getSkin()->textBackgroundColor);
        tft.setTextSize(lcd->getSkin()->textFontSize);
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

        ElementBase::debugSerial(debugLocation);
    }
};

#endif
