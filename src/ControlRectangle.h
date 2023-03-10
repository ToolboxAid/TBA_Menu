#ifndef ControlRectangle_h
#define ControlRectangle_h

#include "Arduino.h"

#include "Dimensions.h"
#include "ControlBase.h"
#include "TBA_Macros.h"

#include "LCD.h"

class ControlRectangle : public ControlBase
{

private:
    Dimensions *dimensions;

protected:
public:
    ControlRectangle(const char *name, Dimensions *dimensions) : ControlBase(name)
    {
        this->dimensions = dimensions;
    }

    ~ControlRectangle()
    {
        if (this->dimensions)
        {
            delete this->dimensions;
        }
    }

    void draw()
    {
        tft.fillRoundRect(this->dimensions->getX(), this->dimensions->getY(),
                          this->dimensions->getW(), this->dimensions->getH(),
                          skin->rectangleRadius, skin->rectangleBorderColor);

        uint8_t offset = skin->buttonBorderWidth;

        tft.fillRoundRect(this->dimensions->getX() + offset, this->dimensions->getY() + offset,
                          this->dimensions->getW() - (offset * 2), this->dimensions->getH() - (offset * 2),
                          skin->rectangleRadius, skin->rectangleBackColor);

        /*
        // Would like a transparent/alpha backgroud overlay, but way to slow, is there a faster way???
        // screen overlay
        for (uint16_t x = 0; x < 3; x++)
        {
          tft.drawRoundRect(25 + x, 125 + x, skin->getScreenWidth() - (25 * 2) - (x * 2), skin->getScreenHeight() - 125 - (25 * 2) - (x * 2), 3, TBA_GRAY);
        }

        for (uint16_t y = 125+5; y < skin->getScreenHeight() - (25 * 2)-5; y++)
          for (uint16_t x = 25+5; x < skin->getScreenWidth() - 25-5; x++)
          {
            // tft.drawPixel(x, y, TBA_GRAY,128, 128, tft.readPixel(x, y));
            tft.drawPixel(x, y, TBA_GRAY, 128);
            tft.drawPixel(x, y, TBA_GRAY);
          }
        */
    }

    Dimensions *getDimensions()
    {
        return this->dimensions;
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        this->dimensions->debugSerial("NO_CR");

        ControlBase::debugSerial(debugLocation);
    }
};

#endif
