#ifndef ElementArg_h
#define ElementArg_h

#include "Arduino.h"

#include "ElementBase.h"
#include "Point.h"

#include "TBA_Macros.h"

class ElementArg : public ElementBase
{
private:
    char *value;

protected:
public:
    ElementArg(const char *name, const char *value) : ElementBase(name) //----->call base class
    {
        this->value = (char *)malloc(strlen(value) + 1);
        memcpy(this->value, value, strlen(value) + 1);
        this->value = (char *)this->value;
    }

    ~ElementArg()
    {
        if (this->value)
        {
            free(this->value);
        }
    }

    boolean hasValue()
    {
        return (this->value && this->value[0] != '\0');
    }
    void *getValue()
    {
        return this->value;
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print(F(" value: '0x"));
        Serial.print((unsigned int)this->value, HEX);

        Serial.print(F("' value: '"));
        if (hasValue())
            Serial.print(this->value);
        else
            Serial.print("isNULL");

        Serial.print(F("' "));

        ElementBase::debugSerial(debugLocation);
    }
};

#endif
