/*
  Point.cpp - Library for Point code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#ifndef Point_h
#define Point_h

#include "Arduino.h"
#include "TBA_Macros.h"

class Point
{
private:
  short X;
  short Y;

protected:
public:
  Point()
  {
    this->X = 0;
    this->Y = 0;
  };
  Point(short x, short y)
  {
    this->X = x;
    this->Y = y;
  }

  ~Point()
  {
    //Serial.println("~Point()");
  };

  short getX()
  {
    return this->X;
  }

  short getY()
  {
    return this->Y;
  }

  void debugSerial(const char *debugLocation)
  {
    Serial.print(F(" <"));
    Serial.print(__FILENAME__);
    Serial.print(F("> "));

    Serial.print(F(" X: "));
    Serial.print(this->X);
    Serial.print(F(" Y: "));
    Serial.print(this->Y);

    Serial.print(F("' debug Loc: '"));
    Serial.print(debugLocation);
    Serial.print(F("' "));

    if (debugLocation != "NO_CR")
    {
      Serial.println();
    }
  }
};

#endif
