#ifndef DerivedElement_h
#define DerivedElement_h

#include "Arduino.h"
#include "VirtualBase.h"
#include "C:/Users/davidq/Documents/Arduino/libraries/TBA_Macros/src/TBA_Macros.h"
//#include "TBA_Macros.h"

class DerivedElement : public VirtualBase
{

private:
    String moreData; // Sample field
    uint16_t moreID; // Sample field

protected:

public:

    DerivedElement(String Name, uint16_t externalID, String moreData, uint16_t moreID) : VirtualBase(Name, externalID) //----->call base class
    {
        Serial.print  ("D externalID: ");
        Serial.print  (externalID);
        Serial.print  (" MoreData: ");
        Serial.print  (moreData);
        Serial.print  (" moreID: ");
        Serial.println(moreID);

        this->moreData = moreData;
        this->moreID = moreID;
    }

    DerivedElement(String Name, uint16_t externalID = 0) : VirtualBase(Name, externalID) //----->call base class // Not sure why it's required.
    {
    }

    void toString(String debugLocation)
    {
        VirtualBase::toString(debugLocation); //----->call base class first
        Serial.print  (" <");
        Serial.print  (__FILENAME__);
        Serial.print  ("> ");        
        Serial.print  (" moreData: ");
        Serial.print  (this->moreData);
        Serial.print  (" moreID: ");
        Serial.print  (this->moreID);
        Serial.println();
    }
};

#endif
