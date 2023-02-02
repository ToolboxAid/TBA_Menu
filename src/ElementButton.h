/*
    This Button Element class provides the ability to:
    1) execute static funtion for short press
    2) Change current Page for short press
    3) execute a different static function when long press
    4) Change to a different current Page when long press
*/

#ifndef ElementButton_h
#define ElementButton_h

#include "Arduino.h"

#include "GlobalConst.h"

#include "Point.h"
#include "Dimensions.h"

#include "ElementBase.h"

#include "TBA_Macros.h"

class ElementButton : public ElementBase
{
private:
 
    Dimensions *dimensions;
    // uint8_t fontSize; // used to override default fontSize

    const char *icon;

    const char *value;


    boolean hidden;

    void (*pShortFunction)(const char *value);
    const char *shortPage;

    void (*pLongFunction)(const char *value);
    const char *longPage;

    static uint16_t deboundDelay;

    static unsigned long pressedTime;
    // static unsigned long releaseTime;
    // static unsigned long lastBounceTime;

    // Test rollover of timer
    // static uint16_t pressedTime;
    // static uint16_t releaseTime;
    // static uint16_t lastBounceTime;

protected:


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

    STATE state;
    STYLE style;

    // Use this to create a Button of style : FILE
    ElementButton(const char *name, Dimensions *dimensions,
                  void (*pShortFunction)(const char *), const char *shortPage,
                  void (*pLongFunction)(const char *), const char *longPage,
                  const char *value)
                  :ElementButton (name, dimensions, 
                                pShortFunction, shortPage,
                                pLongFunction, longPage,
                                NULL,value ,false)
                            //: ElementBase(name) // BUTTON
    { /* calls larger constructor first */ 
        // Filename will be loaded to ->name
        // Filefullpath will be loaded to ->value
        this->style = STYLE::FILE;
    }

    // Use this to create a Button of style : BUTTON
    ElementButton(const char *name, Dimensions *dimensions,
                  void (*pShortFunction)(const char *), const char *shortPage,
                  void (*pLongFunction)(const char *), const char *longPage)
                  :ElementButton (name, dimensions, 
                                pShortFunction, shortPage,
                                pLongFunction, longPage,
                                NULL,NULL,false)
                            //: ElementBase(name) // BUTTON
    { /* calls larger constructor first */ }

    // Use this to create a Button of style : BUTTON
    ElementButton(const char *name, Dimensions *dimensions,
                  void (*pShortFunction)(const char *), const char *shortPage,
                  void (*pLongFunction)(const char *), const char *longPage,
                  const char *icon,
                  const char *value,
                  bool hidden = false) : ElementBase(name) // BUTTON
    {
        this->dimensions = dimensions;

        this->pShortFunction = pShortFunction;
        this->shortPage = shortPage;

        this->value = value;

        this->pLongFunction = pLongFunction;
        this->longPage = longPage;

        this->icon = icon;

        this->state = ElementButton::STATE::UP;  // this->state = (ElementButton::STATE)random(0,6); //this->state = (ElementButton::STATE)random(0,5);

        this->style = STYLE::BUTTON;

        this->hidden = hidden;
    }

    ~ElementButton()
    {
        Serial.println("~ElementButton()");

        icon = NULL;

        value = NULL;

        shortPage = NULL;
        pShortFunction = NULL;

        longPage = NULL;
        pLongFunction = NULL;

        Serial.print(" dim 1 '0x");
        Serial.print((unsigned int)&dimensions, HEX);
        Serial.print("' dim 2 '0x");
        Serial.print((unsigned int)dimensions, HEX);
        Serial.print("'");

        delete dimensions; // We created it, we must delete it...
        dimensions = NULL;

        Serial.println();
    }

    Dimensions *getDimensions()
    {
        return this->dimensions;
    }

    const char *getIcon()
    {
        return this->icon;
    }

    const char *getValue()
    {
        this->value;
    }

    ElementButton::STATE getButtonState()
    {
        return this->state;
    }

    bool isUP()
    {
        return (this->state == ElementButton::STATE::UP);
    }
    bool isSHORT()
    {
        return (this->state == ElementButton::STATE::SHORT);
    }
    bool isLONG()
    {
        return (this->state == ElementButton::STATE::LONG);
    }
    bool isROLLOFF()
    {
        return (this->state == ElementButton::STATE::ROLLOFF);
    }
    bool isRELEASED()
    {
        return ((this->state == ElementButton::STATE::RELEASED));
    }

    bool isStyleButton()
    {
        return (this->style == ElementButton::STYLE::BUTTON);
    }
    bool isStyleFile()
    {
        return (this->style == ElementButton::STYLE::FILE);
    }

    bool isHidden()
    {
        return this->hidden;
    }

    const char *getShortPage()
    {
        return this->shortPage;
    }
    const char *getLongPage()
    {
        return this->longPage;
    }

    void resetButton()
    {
        this->state = ElementButton::STATE::UP;
    }

    /* UP = 0, SHORT, LONG, ROLLOFF, RELEASED */
    bool hasStateChange(Point *point, bool isPressed, ElementButton::STATE &triggerState)
    {
        bool stateChange = false;
        triggerState = ElementButton::STATE::UP;

        if (point->getX() > dimensions->getX() && point->getX() < dimensions->getXW() &&
            point->getY() > dimensions->getY() && point->getY() < dimensions->getYH())
        {
            if (isPressed)
            {
                if (isUP())
                {
                    pressedTime = millis();
                    this->state = ElementButton::STATE::SHORT;
                    stateChange = true;
                }
                if (isSHORT() && millis() > pressedTime + deboundDelay)
                {
                    if (this->pLongFunction != NULL || this->longPage != NULL)
                    {
                        this->state = ElementButton::STATE::LONG;
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
                    this->state = ElementButton::STATE::RELEASED;
                    stateChange = true;
                }
            }
        }
        else
        { // Nothing pressed, did we rolloff?
            if (isSHORT() || isLONG())
            {
                this->state = ElementButton::STATE::ROLLOFF;
                stateChange = true;
            }
        }
        return stateChange;
    }

    boolean executeShortFunction()
    { // check if refreshFunction available
        if (this->pShortFunction != NULL)
        {
            this->pShortFunction(this->value); // do some work on a specific page
            return true;
        }
        return false;
    }
    boolean executeLongFunction()
    { // check if refreshFunction available
        if (this->pLongFunction != NULL)
        {
            this->pLongFunction(this->value); // do some work on a specific page
            return true;
        }
        return false;
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print(F(" STYLE: '"));
        Serial.print(this->style);

        Serial.print(F("' icon: '"));
        Serial.print(this->icon);

        Serial.print(F("' Val: '"));
        Serial.print(this->value);

        Serial.print(F("' St: '"));
        Serial.print(this->state);

        Serial.print(F("' isHid: '"));
        Serial.print(this->hidden);

        Serial.print(F("' shPage: '"));
        Serial.print(this->shortPage);

        Serial.print(F("' lgPage: '"));
        Serial.print(this->longPage);

        Serial.print(F("' "));

        ElementBase::debugSerial(debugLocation);
    }
};

uint16_t ElementButton::deboundDelay = 1000;
unsigned long ElementButton::pressedTime = 0;

#endif
