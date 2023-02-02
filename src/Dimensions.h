/*
  Point.cpp - Library for Point code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#ifndef Dimensions_h
#define Dimensions_h

#include "Arduino.h"
#include "Point.h"
#include "TBA_Macros.h"

class Dimensions : public Point
{

protected:
private:
    //Dimensions(){ };
    short H;
    short W;
 //   bool pressed; // Pressed will be set true if there is a valid touch on the screen

public:
    Dimensions(short x, short y, short w, short h) : Point(x, y) 
    {
        this->H = h;
        this->W = w;
    //  this->pressed = false; // Point(x, y, false)  :: defaults pressed to 29, why?    
    }

    ~Dimensions()
    { 
        Serial.println("~Dimensions()");    
    }

    short getW()
    {
        return this->W;
    }
 
    short getH()
    {
        return this->H;
    }
 
    short getXW()
    {
        return this->getX() + this->W;
    }
 
    short getYH()
    {
        return this->getY() + this->H;
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print(F(" H: '"));
        Serial.print(this->H);
        Serial.print(F("' W: '"));
        Serial.print(this->W);
        Serial.print(F("'"));

        Point::debugSerial(debugLocation); //----->call base class first
    }
};

#endif