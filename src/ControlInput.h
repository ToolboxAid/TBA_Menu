#ifndef ControlInput_h
#define ControlInput_h

#include "Arduino.h"
#include "Dimensions.h"

#include "ControlBase.h"
#include "TBA_Macros.h"

class ControlInput : public ControlBase
{
private:
    Dimensions *dimensions;
    uint8_t length; // is the max length of the array
    uint8_t size;   // is the number of chars appended
    char *input;
    boolean inputChange = false;

protected:
public:
    enum JUSTIFICATION
    { // Do not change order.
        LEFT = 0,
        RIGHT
    };

    JUSTIFICATION justification;

    ControlInput(const char *name, uint8_t length, JUSTIFICATION justification, Dimensions *dimensions, const char *input)
        : ControlInput(name, length, justification, dimensions) //----->call below ControlInput constructor
    {
        // Save the new input
        int loop = 0;
        while (input[loop] != '\0')
        {
            this->append(input[loop++]);
        }
    }

    ControlInput(const char *name, uint8_t length, JUSTIFICATION justification, Dimensions *dimensions)
        : ControlBase(name) //----->call base class
    {
        this->length = length;
        this->size = 0;
        this->input = new char[length + 1]{};
        this->justification = justification;
        this->dimensions = dimensions;

        this->clear();
    }

    ~ControlInput()
    {
        // if (this->input)
        //     free (this->input);
    }

    void draw()
    {
        // tft.setTextColor(skin->textColor, skin->textBackgroundColor);// if drawing background color
        tft.setTextColor(skin->textColor);
        tft.setTextSize(skin->textFontSize);
        tft.setTextDatum(CR_DATUM);

        u_int16_t TBA_GRAY = Skin::rgb888torgb565(0xBBBBBB);
        // Draw input outline
        tft.fillRoundRect(this->dimensions->getX(), this->dimensions->getY(),
                          this->dimensions->getW(), this->dimensions->getH(),
                          skin->buttonRadius,
                          TBA_GRAY);
        // Draw input background
        tft.fillRoundRect(this->dimensions->getX() + skin->buttonBorderWidth, this->dimensions->getY() + skin->buttonBorderWidth,
                          this->dimensions->getW() - (skin->buttonBorderWidth * 2), this->dimensions->getH() - (skin->buttonBorderWidth * 2),
                          skin->buttonRadius,
                          TFT_WHITE);
        // Draw the String
        tft.drawString(this->input,
                       this->dimensions->getX() + this->dimensions->getW() - skin->buttonBorderWidth - skin->buttonPadding,
                       this->dimensions->getY() + (this->dimensions->getH() / 2));
    }

    void clear()
    {
        for (uint8_t loop = 0; loop < length; loop++)
            this->input[loop] = ' ';
        this->input[length] = '\0';
        this->size = 0;
        this->inputChange = true;
    }

    boolean getInputChange()
    {
        return this->inputChange;
    }
    void resetInputChange()
    {
        this->inputChange = false;
    }
    void setInputChange()
    {
        this->inputChange = true;
    }

    void append(const char *append)
    {
        uint8_t start = 0;
        uint8_t end = strlen(append);

        if (end + this->size > this->length)
        {
            start = end - this->length;
        }

        for (uint8_t loop = start; loop < end; loop++)
        {
            this->append(append[loop]);
        }
    }

    void append(char append)
    {
        if (this->size >= this->length)
            return; // input is full

        if (this->justification == JUSTIFICATION::RIGHT)
        {
            for (uint8_t loop = 0; loop < length; loop++)
            {
                this->input[loop] = this->input[loop + 1];
            }
            this->input[length - 1] = append;
            this->input[length] = '\0';
        }
        else
        {
            this->input[size] = append;
        }
        this->size++;
        inputChange = true;
    }

    void back()
    {
        if (this->size == 0)
            return;

        if (this->justification == JUSTIFICATION::RIGHT)
        { // move everything to the right on byte
            for (uint8_t loop = length; loop > 0; loop--)
                this->input[loop] = this->input[loop - 1];
            this->input[length] = '\0';
        }
        else
        { // JUSTIFICATION::LEFT
            // Remove/replace last char on right with ' '
            this->input[this->size - 1] = ' ';
        }
        this->input[0] = ' ';
        inputChange = true;
        this->size--;
    }

    uint8_t getSize()
    {
        return this->size;
    }

    Dimensions *getDimensions()
    {
        return this->dimensions;
    }
    char *getInput()
    {
        return this->input;
    }

    void debugSerial(const char *debugLocation)
    {
        Serial.print(F(" <"));
        Serial.print(__FILENAME__);
        Serial.print(F("> "));

        Serial.print(F(" Length: '"));
        Serial.print(this->length);

        Serial.print(F(" Size: '"));
        Serial.print(this->size);

        Serial.print(F("' input: '"));
        Serial.print(this->input);

        Serial.print(F("' L/R: '"));
        Serial.print(this->justification);

        Serial.print(F("' inputChange: '"));
        Serial.print(this->inputChange);
        Serial.print(F("'  "));

        dimensions->debugSerial("NO_CR");

        ControlBase::debugSerial(debugLocation);
    }
};

#endif
