/*
  PageOK.h - Library for PageOK code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageOK_h
#define PageOK_h

#include "Arduino.h"

#include "Skin.h"

#include "Dimensions.h"
#include "ControlButton.h"
#include "ControlLabel.h"
#include "ControlRectangle.h"

class PageOK : public ControlPage
{
private:
    uint16_t labelID;
    inline static const char *NAME = "PageOK";
    inline static const char *LABEL1 = "File Selected:";

protected:
public:
    PageOK() : ControlPage(NAME, 0, false, false, 5, "Main",NULL)
    {
        Dimensions *dimensions;
        ControlButton *button;
        ControlRectangle *rectangle;

        // button layout
        uint8_t across = 3;
        uint8_t down = 6;

        // =============================================================================
        // Add buttons: clear, Back, Send
        dimensions = skin->getMapDimensions(across, down, 2, 5);
        this->addButton(new ControlButton("OK", dimensions, "Main"));

        // Add label
        this->addLabel(new ControlLabel(LABEL1, new Point(40, 60), false));

        // Add rectangle DW
        uint8_t top = 40;
        uint8_t edg = 25;
        dimensions = new Dimensions(skin->buttonMargin + edg, top, skin->getScreenWidth() - (skin->buttonMargin * 2) - (edg * 2), skin->getScreenHeight() - top - (skin->buttonMargin * 2));
        this->addRectangle(new ControlRectangle("backdrop", dimensions));
    }

    void load()
    {
        ElementArg *argPath = Menu::getInstance()->searchArg("pathSelected");
        if (argPath)
        {
            ControlLabel *label = new ControlLabel((const char *)argPath->getValue(), new Point(40, 90), false);
            argPath->debugSerial("argPath");
        }
        else
        {
            Serial.println("Arg 'pathSelected' Failed");
        }
        
        ElementArg *argFile = Menu::getInstance()->searchArg("fileSelected");
        if (argFile)
        {
            ControlLabel *label = new ControlLabel((const char *)argFile->getValue(), new Point(40, 90), false);
            this->addLabel(label);
            this->labelID = label->getIdentity();
            argFile->debugSerial("argFile");
        }
        else
        {
            Serial.println("Arg 'fileSelected' Failed");
        }
    }

    void exit()
    {
        if (!removeLabel(this->labelID))
        {
            Serial.print("Did not delete labelID: ");
            Serial.println(this->labelID);
        }
    }
};

#endif
