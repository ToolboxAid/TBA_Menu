#ifndef ControlBase_h
#define ControlBase_h

#include "Arduino.h"

#include "ElementBase.h"
#include "Skin.h"

#include "TBA_Macros.h"

#include "LCD.h"

#include "GlobalConst.h"

class ControlBase : public ElementBase
{
private:

    boolean getDisplayHeader()
    {
        return this->displayHeader;
    }

protected:
    Skin *skin;

public:
//Skin *skin,
    ControlBase(const char *name, Skin *skin) : ElementBase(name) //----->call base class
    {
        this->skin = skin;

        // this->value = (char *)malloc(strlen(value) + 1);
        // memcpy(this->value, value, strlen(value) + 1);
        // this->value = (char *)this->value;
        LCD *lcd = LCD::GetInstance();
     }

    ~ControlBase()
    {
        // if (this->value)
        // {
        //     free(this->value);
        // }
    }

    virtual void draw()
    {
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

#endif
