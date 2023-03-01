#ifndef ElementHeader_h
#define ElementHeader_h

#include "Arduino.h"

#include "ElementBase.h"
#include "ElementHeader.h"
#include "Point.h"
#include "Skin.h"


#include "DisplayICON.h"

#include "TBA_Macros.h"

#include "SPI.h"
#include <TFT_eSPI.h> // Hardware-specific library

#include "LCD.h"
#include "GlobalConst.h"

class ElementHeader : public ElementBase
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
    ElementHeader(const char *name, boolean displayHeader) : ElementBase(name) //----->call base class
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
        // {
        //     free(this->value);
        // }
    }

    uint16_t drawIcon()
    {
        LCD *lcd = LCD::GetInstance();

        uint16_t iconOffset = 0;
        if (lcd->getSkin()->headerIconImage)
        {
            int rc, filecount = 0;

            // for (int i = 0; i < sizeof(buffer) / (sizeof(buffer[0])); i++)
            // {
            //   buffer[i] = lcd->getSkin()->headerIconImage[i];
            // }
            // rc = png.open(buffer, myOpen, myClose, myRead, mySeek, PNGDraw);

            rc = png.open(lcd->getSkin()->headerIconImage, myOpen, myClose, myRead, mySeek, PNGDraw);
            if (rc == PNG_SUCCESS)
            {
                iconOffset = lcd->getSkin()->getHeaderWidth() / 2;
                iconOffset -= (strlen(this->getName()) * lcd->getSkin()->getFontWidth() * lcd->getSkin()->headerFontTextSize) / 2;
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
                Serial.println(lcd->getSkin()->headerIconImage);
            }
        }
        return iconOffset;
    }

    void draw()
    {
        LCD *lcd = LCD::GetInstance();

        if (this->getDisplayHeader())
        {
            tft.fillRect(0, 0, lcd->getSkin()->getHeaderWidth(), lcd->getSkin()->getHeaderHeight(), lcd->getSkin()->headerBackGroundColor);
            tft.setTextColor(lcd->getSkin()->headerTextColor, lcd->getSkin()->headerBackGroundColor);
            tft.setTextSize(lcd->getSkin()->headerFontTextSize);

            uint16_t iconOffset = drawIcon();
            // Text last so it may be on top of Icon
            tft.setTextDatum(CC_DATUM);
            tft.drawString(this->getName(),
                           (lcd->getSkin()->getHeaderWidth() / 2) + iconOffset,
                            lcd->getSkin()->getHeaderHeight() / 2);
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

        ElementBase::debugSerial(debugLocation);
    }
};

#endif
