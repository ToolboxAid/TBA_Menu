/*
  Skin.cpp - Library for Skin code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#ifndef Skin_h
#define Skin_h

#include "Arduino.h"
#include "Dimensions.h"

class Skin
{
private:

protected:
public:
   uint16_t screenWidth;
  uint16_t screenHeight;
  uint8_t rotate;

  uint8_t fontWidth;
  uint8_t fontHeight;

  uint16_t headerHeight;
  uint16_t headerWidth;
  
 const char *name;

  // uint8_t  headerFont;    // need to add font someday
  uint8_t headerFontTextSize;
  uint16_t headerTextColor;
  uint16_t headerBackGroundColor;

  // uint8_t  text;
  uint8_t textFontSize;
  uint16_t textColor;
  uint16_t textBackgroundColor;

  // Button
  uint16_t buttonTextColor;
  uint16_t buttonColor;      // color
  uint16_t buttonShortColor; // color short
  uint16_t buttonLongColor;  // color long
  uint16_t buttonBorderColor;
  // File
  uint16_t fileTextColor;
  uint16_t fileBackColor;  // background color
  uint16_t fileShortColor; // color short
  uint16_t fileLongColor;  // color long
  uint16_t fileBorderColor;

  // Button & File
  uint8_t buttonMargin; // spacing between buttons
  uint8_t buttonBorderWidth;
  uint8_t buttonPadding;
  uint8_t buttonRadius;

  // Rectangle
  uint16_t rectangleBackColor;
  uint16_t rectangleBorderColor;
  uint8_t rectangleRadius = 0;

  /* The rotation parameter can be 0, 1, 2 or 3. (Based on SD location)
  NOTE: If you change the rotation, you need to re-calibrate
  Rotation value 0 sets the display to a portrait (tall) mode
  Rotation 1 is landscape (wide) mode
  Rotation value 2 is also a portrait mode
  While rotation 3 is also landscape mode  */
  enum ROTATE
  { // Do not change order.
    SD_UP = 0,
    SD_LEFT,
    SD_DOWN,
    SD_RIGHT,
    Reverse0,
    Reverse1,
    Reverse2,
    Reverse3
  };

  static uint16_t rgb888torgb565(u_int32_t color)
  {
    unsigned char r, g, b;
    uint16_t rgb;
    //                              0x3600AF   > 888  >  565 > 0x3015
    r = (color >> 16 & 0xff); //      36       > 54   >  6   >   3000
    g = (color >> 8 & 0xff);  //        00     > 0    >  0   >     00
    b = (color >> 0 & 0xff);  //          AF   > 175  > 21   >     15

    r = r >> 3;
    g = g >> 2;
    b = b >> 3;

    rgb = (r << 11) | (g << 5) | (b);

    return rgb;
  }

  // This could go in the Button class?
  Dimensions *getMapDimensions(uint8_t buttonsAcross, uint8_t buttonsDown, uint8_t buttonAcross, uint8_t buttonDown)
  {
    uint16_t width = (this->screenWidth - (this->buttonMargin * (buttonsAcross + 1)));
    uint16_t buttonWidth = width / buttonsAcross;
    uint16_t left = (buttonAcross * this->buttonMargin) + ((buttonAcross - 1) * buttonWidth);

    uint16_t height = (this->screenHeight - (this->buttonMargin * (buttonsDown + 1)));
    uint16_t buttonHeight = height / buttonsDown;
    uint16_t top = (buttonDown * this->buttonMargin) + ((buttonDown - 1) * buttonHeight);

    Dimensions *dimension = new Dimensions(left, top, buttonWidth, buttonHeight);
    return dimension;
  }

  Skin()
  { // This skin is the default for TBA Menu
    // Purple    Orange  other   other
    // #3600AF & #ED9700 #B8AEFF #767EFF
    // https://www.rapidtables.com/web/color/color-scheme.html
    // https://www.rapidtables.com/web/color/color-scheme.html

    u_int16_t TBA_ORANGE = Skin::rgb888torgb565(0xED9700);

    u_int16_t TBA_PURPLE = Skin::rgb888torgb565(0x3600AF); // 34%
    u_int16_t TBA_SHORT = Skin::rgb888torgb565(0x4600F0);  // 47%
    u_int16_t TBA_LONG = Skin::rgb888torgb565(0x7433FF);   // 60%

    u_int16_t TBA_NOT_WHITE = Skin::rgb888torgb565(0xEEEEEE);
    u_int16_t TBA_GRAY = Skin::rgb888torgb565(0xAAAAAA);

    this->name = "Toolbox Aid Skin";

    this->rotate = ROTATE::SD_LEFT;
    this->screenWidth = 320;
    this->screenHeight = 240;
    this->fontWidth = 6;
    this->fontHeight = 8;
    /* 6x8 - TFT_getfontsize Returns current font height & width in pixels.
    https://www.esp32.com/viewtopic.php?t=1937
    https://github.com/loboris/ESP32_TFT_library */

    // Header
    this->headerWidth = this->screenWidth;
    this->headerHeight = 30; // fix this, s/b a param
    this->headerFontTextSize = 3;
    this->headerTextColor = TBA_ORANGE;
    this->headerBackGroundColor = TBA_PURPLE;
  
    // Button
    this->buttonTextColor = TBA_ORANGE;
    this->buttonColor = TBA_PURPLE;
    this->buttonShortColor = TBA_SHORT;
    this->buttonLongColor = TBA_LONG;
    this->buttonBorderColor = TBA_GRAY;
    // File
    this->fileTextColor = TBA_PURPLE;
    this->fileBackColor = TBA_NOT_WHITE;
    this->fileShortColor = TBA_SHORT;
    this->fileLongColor = TBA_LONG;
    this->fileBorderColor = TBA_NOT_WHITE;
    // Button & File
    this->buttonMargin = 5;
    this->buttonBorderWidth = 3;
    this->buttonPadding = 3;
    this->buttonRadius = 3;

    // Rectangle
    this->rectangleBackColor = TBA_NOT_WHITE;
    this->rectangleBorderColor = TBA_GRAY;
    this->rectangleRadius = 0;

    // Text
    this->textFontSize = 2;
    this->textColor = TBA_PURPLE;
    this->textBackgroundColor = TBA_ORANGE;
  }

  Skin(const char *name, uint8_t rotate,
       uint16_t screenWidth, uint16_t screenHeight,
       uint8_t headerHeight, uint8_t headerFontTextSize, uint16_t headerTextColor, uint16_t headerBackGroundColor, 
       uint16_t buttonTextColor, uint16_t buttonColor, uint16_t buttonShortColor, uint16_t buttonLongColor, uint16_t buttonBorderColor,
       uint8_t buttonMargin, uint8_t buttonBorderWidth, uint8_t buttonPadding, uint8_t buttonRadius,
       uint8_t textFontSize, uint16_t textColor, uint16_t textBackgroundColor)
  {
    Serial.println("Skin");  //TODO: remove this

    this->name = name;

    // Set the rotation before we calibrate
    if (rotate < 0 || rotate > 7) // if not 0-7, loop forever
    {
      while (true) // the forever loop
      {
        Serial.println("Valid values are: 0, 1, 2, 3, & REVs...");
        delay(1000);
      }
    }
    this->rotate = rotate;

    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->fontWidth = 6;  // next to be added to constructor params.
    this->fontHeight = 8; // next to be added to constructor params.

    // Header
    this->headerWidth = this->screenWidth;
    this->headerHeight = headerHeight;
    this->headerFontTextSize = headerFontTextSize;
    this->headerTextColor = headerTextColor;
    this->headerBackGroundColor = headerBackGroundColor;

    // Button
    this->buttonTextColor = buttonTextColor;
    this->buttonColor = buttonColor;
    this->buttonShortColor = buttonShortColor;
    this->buttonLongColor = buttonLongColor;
    this->buttonBorderColor = buttonBorderColor;
    // // File
    // this->fileTextColor = TBA_PURPLE;
    // this->fileBackColor = TBA_ORANGE;
    // this->fileShortColor = TBA_SHORT;
    // this->fileLongColor = TBA_LONG;
    // this->fileBorderColor = TBA_GRAY;
    // Button & File
    this->buttonMargin = buttonMargin;
    this->buttonBorderWidth = buttonBorderWidth;
    this->buttonPadding = buttonPadding;
    this->buttonRadius = buttonRadius;

    // Text
    this->textFontSize = textFontSize;
    this->textColor = textColor;
    this->textBackgroundColor = textBackgroundColor;
  }

  uint8_t getRotation()
  {
    return this->rotate;
  }
  uint16_t getScreenWidth()
  {
    return this->screenWidth;
  }
  uint16_t getScreenHeight()
  {
    return this->screenHeight;
  }
  uint16_t getHeaderWidth()
  {
    return this->headerWidth;
  }
  uint16_t getHeaderHeight()
  {
    return this->headerHeight;
  }

  uint8_t getFontWidth()
  {
    return this->fontWidth;
  }
  uint8_t getFontHeight()
  {
    return this->fontHeight;
  }

  void debugSerial(const char *debugLocation)
  {
    Serial.print(" <");
    Serial.print(__FILENAME__);
    Serial.print("> ");

    Serial.print(" headerFontTextSize: '");
    Serial.print(this->headerFontTextSize);

    Serial.print("' headerTextColor: '");
    Serial.print(this->headerTextColor);

    Serial.print("' headerBackGroundColor': '");
    Serial.print(this->headerBackGroundColor);

    Serial.print("' H W: '");
    Serial.print(this->getHeaderWidth());

    Serial.print("' H H: '");
    Serial.print(this->getHeaderHeight());

    // Serial.print("' len '");
    // Serial.print(getName().length());

    Serial.print("' F S: '");
    Serial.print(this->headerFontTextSize);

    Serial.print("' F W: '");
    Serial.print(this->getFontWidth());

    Serial.print("' F H: '");
    Serial.print(this->getFontHeight());

    Serial.print(F("' debug Loc: '"));
    Serial.print(debugLocation);
    Serial.print(F("' "));

    Serial.println();
  }
};

#endif
