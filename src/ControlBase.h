#ifndef ControlBase_h
#define ControlBase_h

#include "Arduino.h"

#include "ElementBase.h"

#include "LCD.h"

#include "TBA_Macros.h"

#include "Skin.h"

#include "GlobalTFT.h"

class ControlBase : public ElementBase
{
private:
protected:
    LCD *lcd;
    static Skin *skin;

public:
    ControlBase(const char *name) : ElementBase(name) //----->call base class
    {
        lcd = LCD::GetInstance();
    }

    ~ControlBase()
    {
        // if (this->value)
        //     free(this->value);
    }

    static void setSkin(Skin *skin_);
    Skin *getSkin();

    virtual void draw()
    {
    }

    LCD *getLCD()
    {
        return this->lcd;
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        // Serial.print(F(" value: '0x"));
        // Serial.print((unsigned int)this->value, HEX);

        // Serial.print(F("' value: '"));
        // if (hasValue())
        //     Serial.print(this->value);
        // else
        //     Serial.print("isNULL");

        // Serial.print(F("' "));

        ElementBase::debugSerial(debugLocation);
    }
};

Skin *ControlBase::skin;

void ControlBase::setSkin(Skin *skin_)
{
    skin = skin_;
}
Skin *ControlBase::getSkin()
{
    return skin;
}

#endif
