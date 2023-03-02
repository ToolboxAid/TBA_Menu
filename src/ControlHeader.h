#ifndef ControlHeader_h
#define ControlHeader_h

#include "Arduino.h"

#include "ControlBase.h"
#include "Skin.h"

#include "LCD.h"
#include "LCDicon.h"

#include "TBA_Macros.h"

class ControlHeader : public ControlBase
{
private:
    boolean visible;
    const char *icon;

protected:
public:
    ControlHeader(const char *name, boolean visible, const char *icon) : ControlBase(name) //----->call base class
    {
        this->visible = visible;
        this->icon = icon;
    }

    ~ControlHeader()
    {
    }
    uint16_t drawIcon()
    {
        Serial.println(".....");
        uint16_t iconOffset = 0;
        if (this->icon && this->icon[0] != '\0')
        {
            int rc, filecount = 0;

            rc = png.open(this->icon, myOpen, myClose, myRead, mySeek, PNGDraw);
            if (rc == PNG_SUCCESS)
            {
                iconOffset = getLCD()->getSkin()->getHeaderWidth() / 2;
                iconOffset -= (strlen(this->getName()) * getLCD()->getSkin()->getFontWidth() * getLCD()->getSkin()->headerFontTextSize) / 2;
                iconOffset -= (png.getWidth() / 2) + 2;

                tft.setCursor(iconOffset, 0);

                // Serial.printf("  image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
                rc = png.decode(NULL, 0);
                png.close();

                iconOffset = (png.getWidth() / 2) + 2;
            }
            else
            {
                Serial.print(" Header Icon open failure : ");
                Serial.println(this->icon);
            }
        }
        return iconOffset;
    }

    boolean isVisible()
    {
        return this->visible;
    }

    void draw()
    {
        if (this->visible)
        {
            tft.fillRect(0, 0, getLCD()->getSkin()->getHeaderWidth(), getLCD()->getSkin()->getHeaderHeight(), getLCD()->getSkin()->headerBackGroundColor);
            tft.setTextColor(getLCD()->getSkin()->headerTextColor, getLCD()->getSkin()->headerBackGroundColor);
            tft.setTextSize(getLCD()->getSkin()->headerFontTextSize);

            uint16_t iconOffset = drawIcon();
            // Text last so it may be on top of Icon
            tft.setTextDatum(CC_DATUM);
            tft.drawString(this->getName(),
                           (getLCD()->getSkin()->getHeaderWidth() / 2) + iconOffset,
                           getLCD()->getSkin()->getHeaderHeight() / 2);
        }    
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print(F("visible: '"));
        Serial.print(this->visible);

        // Serial.print(F(" value: '0x"));
        // Serial.print((unsigned int)this->visible, HEX);

        Serial.print(F("' "));

        ControlBase::debugSerial(debugLocation);
    }
};

#endif
