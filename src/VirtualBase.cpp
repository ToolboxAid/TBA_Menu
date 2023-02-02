
// #ifndef VirtualBase_cpp
// #define VirtualBase_cpp

// #include "VirtualBase.h"

// uint16_t VirtualBase::identityCounter = 0;

// VirtualBase::VirtualBase(String Name, uint16_t externalID)
// {
//     //Serial.println("VirtualBase");
//     this->identity = ++VirtualBase::identityCounter;
//     this->Name = Name;
//     this->externalID = externalID;
// }

// uint16_t VirtualBase::getIdentity()
// {
//     return this->identity;
// }

// String VirtualBase::getName()
// {
//     return this->Name;
// }

// uint16_t VirtualBase::getExternalID()
// {
//     return this->externalID;
// }

// void VirtualBase::toString(String debugLocation)
// {
//     Serial.print  (debugLocation);
    
//     Serial.print  ("<Base> identity: ");
//     Serial.print  (this->identity);

//     Serial.print  (" name: ");
//     Serial.print  (this->getName());

//     Serial.print  (" externalID: ");
//     Serial.print  (this->getExternalID());

//     Serial.print  (" this: ");
//     Serial.print  ((unsigned int)this, HEX);    
// }

// #endif