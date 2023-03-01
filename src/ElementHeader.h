#ifndef ElementHeader_h
#define ElementHeader_h

#include "Arduino.h"

#include "SPI.h"
#include <TFT_eSPI.h> // Hardware-specific library

#include "GlobalConst.h"

#include "ControlBase.h"
#include "Skin.h"

#include "LCD.h"
#include "LCDicon.h"

#include "TBA_Macros.h"

class ElementHeader : public ControlBase
{
private:
    boolean displayHeader;
    Skin *skin;
    //    const char * icon;

    boolean getDisplayHeader()
    {
        return this->displayHeader;
    }

protected:
public:
    // Skin *skin,
    ElementHeader(const char *name, boolean displayHeader) : ControlBase(name) //----->call base class
    {
        this->displayHeader = displayHeader;
        this->skin = skin;

        // this->value = (char *)malloc(strlen(value) + 1);
        // memcpy(this->value, value, strlen(value) + 1);
        // this->value = (char *)this->value;
    }

    ~ElementHeader()
    {
        // if (this->value)
        //     free(this->value);
    }

    uint16_t drawIcon()
    {
        uint16_t iconOffset = 0;
        if (getLCD()->getSkin()->headerIconImage)
        {
            int rc, filecount = 0;

            // for (int i = 0; i < sizeof(buffer) / (sizeof(buffer[0])); i++)
            // {
            //   buffer[i] = getLCD()->getSkin()->headerIconImage[i];
            // }
            // rc = png.open(buffer, myOpen, myClose, myRead, mySeek, PNGDraw);

            rc = png.open(getLCD()->getSkin()->headerIconImage, myOpen, myClose, myRead, mySeek, PNGDraw);
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
                Serial.println(getLCD()->getSkin()->headerIconImage);
            }
        }
        return iconOffset;
    }

    void draw()
    {
        if (this->getDisplayHeader())
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

        // Serial.print(F(" value: '0x"));
        // Serial.print((unsigned int)this->value, HEX);

        // Serial.print(F("' value: '"));
        // if (hasValue())
        //     Serial.print(this->value);
        // else
        //     Serial.print("isNULL");

        // Serial.print(F("' "));

        ControlBase::debugSerial(debugLocation);
    }
};

#endif
