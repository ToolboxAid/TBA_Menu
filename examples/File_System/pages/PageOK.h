/*
  PageOK.h - Library for PageOK code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef PageOK_h
#define PageOK_h

#include "Arduino.h"

#include "GlobalConst.h"

#include "Skin.h"

#include "Dimensions.h"
#include "ElementButton.h"
#include "ElementLabel.h"
#include "ElementRectangle.h"

class PageOK : public ElementPage
{
private:
    uint16_t labelID;
    inline static const char *NAME = "PageOK";
    inline static const char *LABEL1 = "File Selected:";

protected:
public:
    PageOK(Skin *skin) : ElementPage(NAME, 0, false, false, 10, "Main")
    {
        Dimensions *dimensions;
        ElementButton *button;
        ElementRectangle *rectangle;

        // button layout
        uint8_t across = 3;
        uint8_t down = 6;

        // =============================================================================
        // Add buttons: clear, Back, Send
        dimensions = skin->getMapDimensions(across, down, 2, 5);
        this->addButton(new ElementButton("OK", dimensions, "Main"));

        // Add label
        this->addLabel(new ElementLabel(LABEL1, new Point(40, 60), false));

        // Add rectangle DW
        uint8_t top = 40;
        uint8_t edg = 25;
        dimensions = new Dimensions(skin->buttonMargin + edg, top, skin->getScreenWidth() - (skin->buttonMargin * 2) - (edg * 2), skin->getScreenHeight() - top - (skin->buttonMargin * 2));
        this->addRectangle(new ElementRectangle("backdrop", dimensions));
    }

    void load()
    {
        ElementArg *arg = Menu::getInstance()->searchArg("buttonSelected");
        if (arg)
        {
            ElementLabel *label = new ElementLabel((const char *)arg->getValue(), new Point(40, 90), false);
            this->addLabel(label);
            this->labelID = label->getIdentity();
        }
        else
        {
            Serial.println("arg 'buttonSelected' Failed");
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
