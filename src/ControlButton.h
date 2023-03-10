/*
    This Button Element class provides the ability to:
    1) execute static funtion for short press
    2) Change current Page for short press
    3) execute a different static function when long press
    4) Change to a different current Page when long press
*/

#ifndef ControlButton_h
#define ControlButton_h

#include "Arduino.h"

#include "LCD.h"
#include "LCDicon.h"

#include "Point.h"
#include "Dimensions.h"

#include "ControlBase.h"

#include "TBA_Macros.h"



class ControlButton : public ControlBase
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

private:
    const char *icon;

    boolean hidden;

    const char *shortPage;

    const char *longPage;

    static uint16_t deboundDelay;
    static unsigned long pressedTime;

protected:
    Dimensions *dimensions;
    const char *value;

    STATE state;
    STYLE style;

    // uint8_t fontSize; // used to override default fontSize

public:
    // Filename will be loaded to ->name
    ControlButton(const char *name, Dimensions *dimensions, const char *shortPage)
        : ControlButton(name, dimensions, shortPage, NULL, NULL)
    { /* calls below larger constructor first */
    }

    ControlButton(const char *name, Dimensions *dimensions, const char *shortPage, const char *longPage, const char *value)
        : ControlButton(name, dimensions, shortPage, longPage, value, NULL, false)
    { /* calls below larger constructor first */
    }

    ControlButton(const char *name, Dimensions *dimensions, const char *shortPage, const char *longPage, const char *value, const char *icon, boolean hidden = false)
        : ControlBase(name)
    {
        this->dimensions = dimensions;

        this->shortPage = shortPage;

        this->longPage = longPage;

        this->value = value;

        this->icon = icon;

        this->hidden = hidden;

        this->state = ControlButton::STATE::UP; // this->state = (ControlButton::STATE)random(0,6); //this->state = (ControlButton::STATE)random(0,5);

        this->style = STYLE::BUTTON;
    }

    ~ControlButton()
    {
        // Serial.print(" dim 1 '0x");
        // Serial.print((unsigned int)&dimensions, HEX);
        // Serial.print("' dim 2 '0x");
        // Serial.print((unsigned int)dimensions, HEX);
        // Serial.println("'");

        delete dimensions; // We created it, we must delete it...
    }

    //=============================================================================================================

    boolean drawButtonIcon()
    {
        if (this->hasIcon())
        {
            int rc, filecount = 0;
            // this->getIcon();
            // for (int i = 0; i < sizeof(buffer) / (sizeof(buffer[0])); i++)
            // {
            //   buffer[i] = this->getIcon()[i];
            // }
            // rc = png.open(buffer, myOpen, myClose, myRead, mySeek, PNGDraw);

            rc = png.open(this->getIcon(), myOpen, myClose, myRead, mySeek, PNGDraw);
            if (rc == PNG_SUCCESS)
            {

                tft.setCursor(this->dimensions->getX() + ((this->dimensions->getW() - png.getWidth()) / 2),
                              this->dimensions->getY() + ((this->dimensions->getH() - png.getHeight()) / 2));

                // Serial.printf("  image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
                rc = png.decode(NULL, 0);
                png.close();
            }
            else
            {
                Serial.print(" Button Icon open failure: '");
                Serial.print(this->getIcon());
                Serial.println("'");
                return false;
            }
            return true;
        }
        return false;
    }

    uint16_t getStateColor()
    {
        uint16_t buttonColor;

        switch (this->state)
        {
        case ControlButton::STATE::UP:
        case ControlButton::STATE::ROLLOFF:
        case ControlButton::STATE::RELEASED:
            buttonColor = skin->buttonColor;
            break;
        case ControlButton::STATE::SHORT:
            buttonColor = skin->buttonShortColor;
            break;
        case ControlButton::STATE::LONG:
            buttonColor = skin->buttonLongColor;
            break;
        default:
            buttonColor = Skin::rgb888torgb565(0xFF0000);
            Serial.print(F("Invalid Button STATE: "));
            Serial.print(this->getButtonState());
            Serial.print(F(" for Button : "));
            Serial.println(this->getName());
            break;
        }
        return buttonColor;
    }
    void draw()
    {
        if (!this->isHidden())
        {
            tft.setTextSize(skin->textFontSize);

            uint16_t buttonColor = getStateColor();

            // Draw button outline
            tft.fillRoundRect(this->dimensions->getX(), this->dimensions->getY(),
                              this->dimensions->getW(), this->dimensions->getH(),
                              skin->buttonRadius, skin->buttonBorderColor);

            // Draw button background
            tft.fillRoundRect(this->dimensions->getX() + skin->buttonBorderWidth, this->dimensions->getY() + skin->buttonBorderWidth,
                              this->dimensions->getW() - (skin->buttonBorderWidth * 2), this->dimensions->getH() - (skin->buttonBorderWidth * 2),
                              skin->buttonRadius, buttonColor);

            // Draw Icon
            if (!drawButtonIcon())
            {
                // Draw Name
                tft.setTextColor(skin->buttonTextColor, buttonColor);
                tft.setTextDatum(CC_DATUM);
                tft.drawString(this->getName(),
                               this->dimensions->getX() + (this->dimensions->getW() / 2),
                               this->dimensions->getY() + (this->dimensions->getH() / 2));
            }
        }
    }

    //=============================================================================================================

    Dimensions *getDimensions()
    {
        return this->dimensions;
    }

    boolean hasIcon()
    {
        return (this->icon && this->icon[0] != '\0');
    }
    const char *getIcon()
    {
        return this->icon;
    }

    boolean hasValue()
    {
        if (this->value)
            if (this->value[0] != '\0')
                return true;
        return false;
    }
    virtual const char *getValue() // virtual: Just incase you need to reserve memory for this
    {
        return this->value;
    }

    ControlButton::STATE getButtonState()
    {
        return this->state;
    }

    boolean isUP()
    {
        return (this->state == ControlButton::STATE::UP);
    }
    boolean isSHORT()
    {
        return (this->state == ControlButton::STATE::SHORT);
    }
    boolean isLONG()
    {
        return (this->state == ControlButton::STATE::LONG);
    }
    boolean isROLLOFF()
    {
        return (this->state == ControlButton::STATE::ROLLOFF);
    }
    boolean isRELEASED()
    {
        return ((this->state == ControlButton::STATE::RELEASED));
    }

    boolean isStyleButton()
    {
        return (this->style == ControlButton::STYLE::BUTTON);
    }
    boolean isStyleFile()
    {
        return (this->style == ControlButton::STYLE::FILE);
    }

    boolean isHidden()
    {
        return this->hidden;
    }

    boolean hasShortPage()
    {
        if (this->shortPage && this->shortPage[0] != '\0')
            return true;
        return false;
    }
    const char *getShortPage()
    {
        return this->shortPage;
    }
    boolean hasLongPage()
    {
        if (this->longPage && this->longPage[0] != '\0')
            return true;
        return false;
    }
    const char *getLongPage()
    {
        return this->longPage;
    }

    void resetButton()
    {
        this->state = ControlButton::STATE::UP;
    }

    /* UP = 0, SHORT, LONG, ROLLOFF, RELEASED */
    boolean hasStateChange(Point *point, boolean isPressed, ControlButton::STATE &triggerState)
    {
        boolean stateChange = false;
        triggerState = ControlButton::STATE::UP;

        if (point->getX() > dimensions->getX() && point->getX() < dimensions->getXW() &&
            point->getY() > dimensions->getY() && point->getY() < dimensions->getYH())
        {
            if (isPressed)
            {
                if (isUP())
                {
                    pressedTime = millis();
                    this->state = ControlButton::STATE::SHORT;
                    stateChange = true;
                }
                else if (isSHORT() && millis() > pressedTime + deboundDelay)
                {
                    if (this->hasLongPage())
                    {
                        this->state = ControlButton::STATE::LONG;
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
                    this->state = ControlButton::STATE::RELEASED;
                    stateChange = true;
                }
            }
        }
        else
        { // Nothing pressed, did we rolloff?
            if (isSHORT() || isLONG())
            {
                this->state = ControlButton::STATE::ROLLOFF;
                stateChange = true;
            }
        }
        return stateChange;
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        dimensions->debugSerial("NO_CR");

        Serial.print(F("' shortPg: '"));
        if (this->hasShortPage())
            Serial.print(this->shortPage);
        else
            Serial.print("isNULL");

        Serial.print(F("' longPg: '"));
        if (this->hasLongPage())
            Serial.print(this->longPage);
        else
            Serial.print("isNULL");

        Serial.print(F("' value: '"));
        if (this->hasValue())
            Serial.print(this->value);
        else
            Serial.print("isNULL");

        Serial.print(F("' icon: '"));
        if (this->hasValue())
            Serial.print(this->icon);
        else
            Serial.print("isNULL");

        Serial.print(F("' isHid: '"));
        Serial.print(this->hidden);

        Serial.print(F(" STYLE: '"));
        Serial.print(this->style);

        Serial.print(F("' STATE: '"));
        Serial.print(this->state);

        Serial.print(F("' "));
        ControlBase::debugSerial(debugLocation);
    }
};

uint16_t ControlButton::deboundDelay = 1000;
unsigned long ControlButton::pressedTime = 0;

#endif
