/*
    This Button Element class provides the ability to:
    1) execute static funtion for short press
    2) Change current Page for short press
    3) execute a different static function when long press
    4) Change to a different current Page when long press
*/

#ifndef ButtonElement_h
#define ButtonElement_h

#include "Arduino.h"

#include "GlobalConst.h"

#include "Point.h"
#include "Dimensions.h"

#include "VirtualBase.h"

#include "TBA_Macros.h"

class ButtonElement : public VirtualBase
{

public:
  enum STATE
  { // Do not change order.
    UP = 0,
    SHORT,
    LONG,
    ROLLOFF,
    RELEASED
  };
  enum STYLE
  { // Do not change order.
    BUTTON = 0,
    FILE
  };  

    // Use this to create a Button of style : BUTTON
    ButtonElement(std::string name, Dimensions *dimensions, 
                void (*pShortFunction)(std::string), std::string shortPage, 
                void (*pLongFunction)(std::string), std::string longPage,
                std::string icon = EMPTY,
                std::string value = EMPTY,
                bool hidden = false      ) : VirtualBase(name) // BUTTON
    {
        this->dimensions = dimensions;

        this->pShortFunction = pShortFunction;
        this->shortPage = shortPage;

        this->value = value;
        
        this->pLongFunction =  pLongFunction;
        this->longPage = longPage;

        this->icon = icon;

        this->state = ButtonElement::STATE::UP; //this->state = (ButtonElement::STATE)random(0,6); //this->state = (ButtonElement::STATE)random(0,5);

        this->style = STYLE::BUTTON;
        
        this->hidden = hidden;
    }

    ~ButtonElement()
    {
        Serial.println("~ButtonElement()");
        
        icon.erase();
        icon.shrink_to_fit();

        value.erase();
        value.shrink_to_fit();

        shortPage.erase();
        shortPage.shrink_to_fit();
pShortFunction = NULL;

        longPage.erase();
        longPage.shrink_to_fit();
pLongFunction = NULL;

        dimensions = NULL;

Dimensions *d = dimensions;
Serial.print(" '");Serial.print((unsigned int)&d,HEX);Serial.print("'");
// delete d;
// Serial.print(" '");Serial.print((unsigned int)&d,HEX);Serial.print("'");
// d = NULL;
// Serial.print(" '");Serial.print((unsigned int)&d,HEX);Serial.print("'");
Serial.println();
    }

    Dimensions *getDimensions()
    {
        return this->dimensions;
    }

    std::string getIcon()
    {
        return this->icon;
    }

    std::string getValue()
    {
        this->value;
    }

    ButtonElement::STATE getButtonState()
    {
        return this->state;
    }

    bool isUP()
    {
        return (this->state == ButtonElement::STATE::UP);
    }
    bool isSHORT()
    {
        return (this->state == ButtonElement::STATE::SHORT);
    }    
    bool isLONG()
    {
        return (this->state == ButtonElement::STATE::LONG);
    }
    bool isROLLOFF()
    {
        return (this->state == ButtonElement::STATE::ROLLOFF);
    }    
    bool isRELEASED()
    {
        return ((this->state == ButtonElement::STATE::RELEASED));
    }        

    bool isStyleButton()
    {
        return (this->style == ButtonElement::STYLE::BUTTON);
    }
    bool isStyleFile()
    {
        return (this->style == ButtonElement::STYLE::FILE);
    }

    bool isHidden()
    {
        return this->hidden;
    }

    std::string getShortPage()
    {
        return this->shortPage;
    }
    std::string getLongPage()
    {
        return this->longPage;
    }

    void resetButton()
    {
        this->state = ButtonElement::STATE::UP;
    }

    /* UP = 0, SHORT, LONG, ROLLOFF, RELEASED */
    bool hasStateChange(Point *point, bool isPressed, ButtonElement::STATE &triggerState)
    {
        bool stateChange = false;
        triggerState = ButtonElement::STATE::UP;

        if (point->getX() > dimensions->getX() && point->getX() < dimensions->getXW() &&
            point->getY() > dimensions->getY() && point->getY() < dimensions->getYH())
        {
            if (isPressed)
            {
                if (isUP())
                {
                    pressedTime = millis();
                    this->state = ButtonElement::STATE::SHORT;
                    stateChange = true;
                }
                if (isSHORT() && millis() > pressedTime + deboundDelay)
                {
                    if (this->pLongFunction != NULL || !this->longPage.empty())
                    {
                    this->state = ButtonElement::STATE::LONG;
                    stateChange = true;
                    }
                }
            }
            else
            {
                if (!isUP())
                {
                    // On button but not pressed
                    // Set current state for return. (is short or long)
                    triggerState = this->state;
                    this->state = ButtonElement::STATE::RELEASED;
                    stateChange = true;
                }
            }
        }
        else
        { // Nothing pressed, did we rolloff?
            if (isSHORT() || isLONG())
            {
                this->state = ButtonElement::STATE::ROLLOFF;
                stateChange = true;
            }
        }
        return stateChange;
    }

    boolean executeShortFunction()
    {   // check if refreshFunction available
        if (this->pShortFunction != NULL)
        {
            this->pShortFunction(this->value); // do some work on a specific page
            return true;
        }
        return false;
    }
    boolean executeLongFunction()
    {   // check if refreshFunction available
        if (this->pLongFunction != NULL)
        {
            this->pLongFunction(this->value); // do some work on a specific page
            return true;
        }
        return false;
    }

    void debugSerial(std::string debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print (F(" STYLE: '"));
        Serial.print (this->style);

        Serial.print (F("' icon: '"));
        Serial.print (this->icon.c_str());

        Serial.print(F("' Val: '"));
        Serial.print(this->value.c_str());

        Serial.print(F("' St: '"));
        Serial.print(this->state);

        Serial.print(F("' isHid: '"));
        Serial.print(this->hidden);

        Serial.print(F("' shPage: '"));
        Serial.print(this->shortPage.c_str());

        Serial.print(F("' lgPage: '"));
        Serial.print(this->longPage.c_str());

        Serial.print(F("' "));

        this->dimensions->debugSerial("NO_CR");

        VirtualBase::debugSerial(debugLocation);
    }

protected:
    STYLE style;

private:
    Dimensions *dimensions;
    // uint8_t fontSize; // used to override default fontSize
    
    std::string icon;

    std::string value;

    STATE state;

    boolean hidden;

    void (*pShortFunction)(std::string value);
    std::string shortPage;

    void (*pLongFunction)(std::string value);
    std::string longPage;

    static uint16_t deboundDelay;

    static unsigned long pressedTime;
    // static unsigned long releaseTime;
    //static unsigned long lastBounceTime;

    // Test rollover of timer
    // static uint16_t pressedTime;
    // static uint16_t releaseTime;
    // static uint16_t lastBounceTime;


};

uint16_t ButtonElement::deboundDelay = 1000;
unsigned long ButtonElement::pressedTime = 0;
// unsigned long ButtonElement::releaseTime = 0;
//unsigned long ButtonElement::lastBounceTime = 0;

#endif
