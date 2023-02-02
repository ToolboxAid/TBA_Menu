// #ifndef TextElement_h
// #define TextElement_h

// #include "Arduino.h"
// #include "Point.h"

// #include "VirtualBase.h"
// #include "TBA_Macros.h"

// class TextElement : public VirtualBase
// {

// private:
//     Point *point;

// protected:
// public:
//     TextElement(std::string name, Point *point) : VirtualBase(name)
//     {
//         this->point = point;
//     }

//     Point *getPoint()
//     {
//         return this->point;
//     }

//     void debugSerial(std::string debugLocation)
//     {
//         Serial.print(F(" <"));
//         Serial.print(__FILENAME__);
//         Serial.print(F("> "));

//         this->point->debugSerial("' 4Text");

//         VirtualBase::debugSerial(debugLocation);
//     }
// };

// #endif
