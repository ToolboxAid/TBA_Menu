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
    char *display;

protected:
public:
    ElementVariable(const char *name, void *value, const char *format, char *display, Point *point) : ElementBase(name) //----->call base class
    {
        this->point = point;
        this->value = value;
        this->format = format;
        this->display = display;
    }

    ~ElementVariable()
    {
        delete point;
        if (value)
            delete value;
        if (format)
            delete format;
        if (display)
            delete display;
    }

    void draw()
    {
        LCD *lcd = LCD::GetInstance();

        tft.setTextColor(lcd->getSkin()->textColor, lcd->getSkin()->textBackgroundColor);
        tft.setTextSize(lcd->getSkin()->textFontSize);
        tft.setTextDatum(TL_DATUM);

        tft.setCursor(this->point->getX(), this->point->getY());
        tft.print(this->display);
    }

    void *getValue()
    {
        return this->value;
    }
    const char *getFormat()
    {
        return this->format;
    }
    char *getDisplay()
    {
        return this->display;
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

        Serial.print(F("' display: '0x"));
        Serial.print((unsigned int)display, HEX);
        Serial.print(F("' display: '"));
        Serial.print(display);

        Serial.print(F("' "));

        ElementBase::debugSerial(debugLocation);
    }
};

#endif
