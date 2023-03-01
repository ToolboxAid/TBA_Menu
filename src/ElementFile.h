/*
    This Button Element class provides the ability to:
    1) execute static funtion for short press
    2) Change current Page for short press
    3) execute a different static function when long press
    4) Change to a different current Page when long press
*/

#ifndef ElementFile_h
#define ElementFile_h

#include "Arduino.h"

#include "LCD.h"
#include "GlobalConst.h"

#include "Point.h"
#include "Dimensions.h"

#include "ElementButton.h"

#include "TBA_Macros.h"

class ElementFile : public ElementButton
{
private:
    char *nameBuffer;
    boolean directory;

protected:
public:
    // Use this to create a Button of style : FILE
    ElementFile(const char *name, Dimensions *dimensions, const char *shortPage, const char *icon, boolean directory)
        : ElementButton(name, dimensions, shortPage)
    {
        this->style = STYLE::FILE;
        this->directory = directory;

        nameBuffer = (char *)malloc(strlen(name) + 2);
        memcpy(nameBuffer, name, strlen(name) + 1);
        this->name = nameBuffer;
    }

    ~ElementFile()
    {
        if (nameBuffer)
            free(nameBuffer);
    }

    //=============================================================================================================
    uint16_t getFileStateBoarderColor()
    {
        uint16_t fileTextColor;

        switch (this->state)
        {
        case ElementButton::STATE::UP:
        case ElementButton::STATE::ROLLOFF:
        case ElementButton::STATE::RELEASED:
            fileTextColor = getLCD()->getSkin()->fileBorderColor;
            break;
        case ElementButton::STATE::SHORT:
            fileTextColor = getLCD()->getSkin()->fileShortColor;
            break;
        case ElementButton::STATE::LONG:
            fileTextColor = getLCD()->getSkin()->fileLongColor;
            break;
        default:
            fileTextColor = Skin::rgb888torgb565(0xFF0000);
            Serial.print(F("Invalid Button STATE: "));
            Serial.print(this->getButtonState());
            Serial.print(F(" for Button : "));
            Serial.println(this->getName());
            break;
        }
        return fileTextColor;
    }
    uint16_t getFileStateColor()
    {
        uint16_t fileTextColor;

        switch (this->state)
        {
        case ElementButton::STATE::UP:
        case ElementButton::STATE::ROLLOFF:
        case ElementButton::STATE::RELEASED:
            fileTextColor = getLCD()->getSkin()->fileTextColor;
            break;
        case ElementButton::STATE::SHORT:
            fileTextColor = getLCD()->getSkin()->fileShortColor;
            break;
        case ElementButton::STATE::LONG:
            fileTextColor = getLCD()->getSkin()->fileLongColor;
            break;
        default:
            fileTextColor = Skin::rgb888torgb565(0xFF0000);
            Serial.print(F("Invalid Button STATE: "));
            Serial.print(this->getButtonState());
            Serial.print(F(" for Button : "));
            Serial.println(this->getName());
            break;
        }
        return fileTextColor;
    }

    void draw()
    {
        // Draw Icon
        if (!drawButtonIcon())
        {
            // Draw Name
            tft.setTextSize(getLCD()->getSkin()->textFontSize);
            tft.setTextColor(getFileStateColor(), getLCD()->getSkin()->fileBackColor, true);
            tft.setTextDatum(TL_DATUM);

            //(strlen(currentPage->getName()) * getLCD()->getSkin()->getFontWidth() * getLCD()->getSkin()->headerFontTextSize) / 2;
            uint8_t end = strlen(this->getName());
            uint8_t numChar = this->dimensions->getXW() / (6 * 2) - 1;

            if (end > numChar)
                end = numChar;

            // give me some space
            char *ptr;
            ptr = (char *)malloc(end * sizeof(char) + 1);
            memcpy(ptr, this->getName(), end);
            ptr[end] = '\0';

            tft.drawString(ptr, this->dimensions->getX() + 3, this->dimensions->getY() + 3);

            // give space back
            free(ptr);
        }

        tft.drawRoundRect(this->dimensions->getX() - 1, this->dimensions->getY() + 1,
                          this->dimensions->getW(), this->dimensions->getH(),
                          getLCD()->getSkin()->buttonRadius,
                          getFileStateBoarderColor());
    }
    //=============================================================================================================

    boolean isDirectory()
    {
        return this->directory;
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print(F(" Name: '"));
        Serial.print(this->nameBuffer);

        Serial.print(F("' isDirectory '"));
        if (this->isStyleFile())
            Serial.print(this->directory);
        else
        {
            Serial.print(F("!"));
        }
        Serial.print(F("' "));

        ControlBase::debugSerial(debugLocation);
    }
};

#endif
