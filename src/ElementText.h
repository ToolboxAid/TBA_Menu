// #ifndef ElementText_h
// #define ElementText_h

// #include "Arduino.h"
// #include "Point.h"

// #include "ElementBase.h"
// #include "TBA_Macros.h"

// class ElementText : public ElementBase
// {

// private:
//     Point *point;

// protected:
// public:
//     ElementText(const char * name, Point *point) : ElementBase(name)
//     {
//         this->point = point;
//     }

//     Point *getPoint()
//     {
//         return this->point;
//     }

//     void debugSerial(const char * debugLocation)
//     {
//         Serial.print(F(" <"));
//         Serial.print(__FILENAME__);
//         Serial.print(F("> "));

//         this->point->debugSerial("' 4Text");

//         ElementBase::debugSerial(debugLocation);
//     }
// };

// #endif
