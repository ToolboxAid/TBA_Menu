#ifndef VirtualBase_cpp
#define VirtualBase_cpp

//#include "C:/Users/davidq/Documents/Arduino/libraries/TBA_Macros/src/TBA_Macros.h"
#include "../../TBA_Macros/src/TBA_Macros.h"
//#include "TBA_Macros.h"

class VirtualBase
{
protected:
private:
    uint16_t identity;

    String name;
    uint16_t externalID; // max value 65535

public:
    static uint16_t identityCounter; // inits to 0;

    VirtualBase(String name, uint16_t externalID = 0)
    {
        this->identity = ++VirtualBase::identityCounter;
        this->name = name;
        if (externalID == 0) // if no external ID, use the identity
        { 
            this->externalID = identity; 
        }
        else
        {
            this->externalID = externalID;
        }
    }

    uint16_t getIdentity()
    {
        return this->identity;
    }

    String getName()
    {
        return this->name;
    }

    uint16_t getExternalID()
    {
        return this->externalID;
    }

    void toString(String debugLocation)
    {
        Serial.print(debugLocation);
        Serial.print(" <");
        Serial.print(__FILENAME__);
        Serial.print("> ");

        Serial.print("identity: ");
        Serial.print(this->identity);

        Serial.print(" name: ");
        Serial.print(this->getName());

        Serial.print(" externalID: ");
        Serial.print(this->getExternalID());

        Serial.print(" this: ");
        Serial.print((unsigned int)this, HEX);
    }
};

uint16_t VirtualBase::identityCounter = 0;

#endif
