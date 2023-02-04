#ifndef ElementInput_h
#define ElementInput_h

#include "Arduino.h"
#include "Dimensions.h"

#include "ElementBase.h"
#include "TBA_Macros.h"

class ElementInput : public ElementBase
{
private:
    Dimensions *dimensions;
    uint8_t length;
    char *input;

    boolean inputChange = false;

protected:
public:
    ElementInput(const char *name, uint8_t length, Dimensions *dimensions) : ElementBase(name) //----->call base class
                                                                                               //  ElementInput(const char *name, char * input, Dimensions *dimensions) : ElementBase(name) //----->call base class
    {
        this->dimensions = dimensions;
        this->length = length;
        this->input = new char[length]{};
        this->clear();

        /*
        Serial.println(strlen(this->input));
        Serial.print  ("this->input: '");
        Serial.print  (this->input);
        Serial.println("'");
        Serial.println("              1234567890123456789012345678901234567890");
        Serial.println("              0        1         2         3         4");
        */
    }

    ~ElementInput()
    {
        if (this->input)
            delete this->input;
    }

    void clear()
    {
        for (uint8_t loop = 0; loop < length; loop++)
            this->input[loop] = ' ';
        this->input[length] = '\0';
        inputChange = true;
    }

    boolean getInputChange()
    {
        return this->inputChange;
    }
    void resetInputChange()
    {
        this->inputChange = false;
    }

    void append(const char *append)
    {
        for (uint8_t loop = 0; loop < length; loop++)
            this->input[loop] = this->input[loop + 1];

        this->input[length - 1] = append[0];
        // this->input[length - 1] = std::string(append);
        // strncpy(this->input[length - 1], append, 1);
        this->input[length] = '\0';

        inputChange = true;
    }

    void append(char append)
    {
        for (uint8_t loop = 0; loop < length; loop++)
            this->input[loop] = this->input[loop + 1];

        this->input[length - 1] = append;
        // this->input[length - 1] = std::string(append);
        // strncpy(this->input[length - 1], append, 1);
        this->input[length] = '\0';

        inputChange = true;
    }

    void back()
    {
        for (uint8_t loop = length; loop > 0; loop--)
            this->input[loop] = this->input[loop - 1];

        this->input[0] = ' ';
        this->input[length] = '\0';
        inputChange = true;
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
        Serial.print(length);

        Serial.print(F("' input: '"));
        Serial.print(input);

        Serial.print(F("' inputChange: '"));
        Serial.print(inputChange);
        Serial.print(F("'  "));

        dimensions->debugSerial("NO_CR");

        ElementBase::debugSerial(debugLocation);
    }
};

#endif
