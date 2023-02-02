/*
  Display.h - Library for Display code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

/*
Pre-Defined TFT_eSPI colors located in:
C:\Users\davidq\Documents\Arduino\libraries\TFT_eSPI  line 295
*/

#pragma once

#ifndef Display_h
#define Display_h

#include "GlobalConst.h"

#include "Arduino.h"

/* The SPIFFS (FLASH file system) is used
   to hold touch screen calibration data
*/

#include "FS.h"
#include <SPI.h>

#include "TBA_FileSystem.h"

#include "ElementPage.h"
#include "Skin.h"
#include "Point.h"

//#include "DisplayPNG.h"
#include "DisplayICON.h"

/* This is the file name used to store the calibration data
   You can change this to create new calibration files.
   The SPIFFS file name must start with "/".
*/
#define CALIBRATION_FILE "/TouchCal.Data"


/* Set REPEAT_CAL to true instead of false to run calibration
   again, otherwise it will only be done once.
   Repeat calibration if you change the screen rotation.
   */
#define REPEAT_CAL false

/* This is a singleton class,
only one instance will ever be created
*/
class Display
{
private:

  static Display *thisDisplay;

  Display() {}
  void touch_calibrate();

  /* Draw below this line */
  uint16_t drawHeaderIcon(ElementPage *currentPage);
  void drawHeader(ElementPage *currentPage);
  void drawLabel(ElementPage *currentPage);
  uint16_t getFileStateColor(ElementButton *button);
  uint16_t getFileStateBoarderColor(ElementButton *button);  
  uint16_t getButtonStateColor(ElementButton *button);
  void drawFile(ElementPage *currentPage, ElementButton *button);
  bool drawButtonIcon(ElementButton *button);
  void drawButtons(ElementPage *currentPage);
  void drawRectangle(ElementPage *currentPage);

  Skin skin;

  // Store the touch coordinates
  uint16_t t_x = 0, t_y = 0;
  Point *point;
  
protected:
public:


  static Display *getInstance()
  {
    if (!thisDisplay)
    {
      thisDisplay = new Display; // ()
      TBA_FileSystem fileSystem = TBA_FileSystem();  // init SD & SPIFFS
    }
    return thisDisplay;
  }

  void init(Skin &skin);
  void drawCurrentPage(ElementPage *currentPage);
  void drawVariable(ElementPage *currentPage);
  void drawInput(ElementPage *currentPage);
  void drawButton(ElementPage *currentPage, ElementButton *button);

  Dimensions *getScreenDimensions();

  Point *getScreenTouchPoint(bool &pressed);
  TFT_eSPI getScreenTFT();
  void debugSerial(const char * debugLocation);
};

/* Allocating and initializing GlobalClass's
   static data member.  The pointer is being
   allocated - not the object inself.
 */
Display *Display::thisDisplay = 0;

void Display::init(Skin &skin)
{
  thisDisplay->skin = skin;
  // Set communication speed
  tft.begin(4000000);

  // Initialise the TFT screen
  tft.init();
  tft.setRotation(thisDisplay->skin.getRotation());

  // Calibrate the touch screen and determin the scaling factors
  touch_calibrate();

  tft.setTextFont(0);
}

void Display::touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE))
  {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f)
      {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL)
  {
    // calibration data valid
    tft.setTouch(calData);
  }
  else
  {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL)
    {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f)
    {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

uint16_t Display::drawHeaderIcon(ElementPage *currentPage)
{
  uint16_t iconOffset = 0;
  if (thisDisplay->skin.headerIconImage  != NULL)
  {
    int rc, filecount = 0;

    for (int i = 0; i < sizeof(buffer) / (sizeof(buffer[0])); i++)
    {
      buffer[i] = thisDisplay->skin.headerIconImage[i];
    }

    rc = png.open((const char *)buffer, myOpen, myClose, myRead, mySeek, PNGDraw);
    if (rc == PNG_SUCCESS)
    {
      iconOffset = thisDisplay->skin.getHeaderWidth() / 2;
      iconOffset -= (strlen(currentPage->getName()) * thisDisplay->skin.getFontWidth() * thisDisplay->skin.headerFontTextSize) / 2;
      iconOffset -= (png.getWidth() / 2) + 2;

      tft.setCursor(iconOffset, 0);

      // Serial.printf("  image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
      rc = png.decode(NULL, 0);
      png.close();

      iconOffset = (png.getWidth() / 2) + 2;
    }
    else
    {
      Serial.print(" Icon open failure: ");
      Serial.println(thisDisplay->skin.headerIconImage);
    }
  }
  return iconOffset;
}
void Display::drawHeader(ElementPage *currentPage)
{

  // Display Header on the screen if required
  if (currentPage->getDisplayHeader())
  { // Draw Header

    tft.fillRect(0, 0, thisDisplay->skin.getHeaderWidth(), thisDisplay->skin.getHeaderHeight(), thisDisplay->skin.headerBackGroundColor);

    tft.setTextColor(thisDisplay->skin.headerTextColor, thisDisplay->skin.headerBackGroundColor);
    tft.setTextSize(thisDisplay->skin.headerFontTextSize);

    uint16_t iconOffset = drawHeaderIcon(currentPage); // 0;
    // Text last so it may be on top of Icon
    tft.setTextDatum(CC_DATUM);
    tft.drawString(currentPage->getName(),
                   (thisDisplay->skin.getHeaderWidth() / 2) + iconOffset,
                   thisDisplay->skin.getHeaderHeight() / 2);
  }
}

void Display::drawLabel(ElementPage *currentPage)
{
  tft.setTextSize(thisDisplay->skin.textFontSize);
  tft.setTextDatum(TL_DATUM);

  // iterator
  ElementLabel *label = currentPage->nextLabel();
  while (label)
  {
    if (label->getDrawBackgroundColor())
    {
      tft.setTextColor(thisDisplay->skin.textColor, thisDisplay->skin.textBackgroundColor);
    }
    else
    {
      tft.setTextColor(thisDisplay->skin.textColor);
    }    
    tft.setCursor(label->getPoint()->getX(), label->getPoint()->getY());
    tft.print(label->getName());
    label = currentPage->nextLabel();
  }
}
void Display::drawVariable(ElementPage *currentPage)
{
  tft.setTextColor(thisDisplay->skin.textColor, thisDisplay->skin.textBackgroundColor);
  tft.setTextSize(thisDisplay->skin.textFontSize);
  tft.setTextDatum(TL_DATUM);

  // iterator
  ElementVariable *variable = currentPage->nextVariable();
  while (variable)
  {
    tft.setCursor(variable->getPoint()->getX(), variable->getPoint()->getY());
    tft.print(variable->getDisplay());
    variable = currentPage->nextVariable();
  }
}

void Display::drawInput(ElementPage *currentPage)
{

  //tft.setTextColor(thisDisplay->skin.textColor, thisDisplay->skin.textBackgroundColor);
  tft.setTextColor(thisDisplay->skin.textColor);
  tft.setTextSize(thisDisplay->skin.textFontSize);
  tft.setTextDatum(CR_DATUM);

  u_int16_t TBA_GRAY = Skin::rgb888torgb565(0xBBBBBB);

  // iterator
  ElementInput *input = currentPage->nextInput();
  while (input)
  {
   // Draw input outline
    tft.fillRoundRect(input->getDimensions()->getX(), input->getDimensions()->getY(),
                      input->getDimensions()->getW(), input->getDimensions()->getH(),
                      thisDisplay->skin.buttonRadius,
                      TBA_GRAY);
    // Draw input background
    tft.fillRoundRect(input->getDimensions()->getX() + thisDisplay->skin.buttonBorderWidth, input->getDimensions()->getY() + thisDisplay->skin.buttonBorderWidth,
                      input->getDimensions()->getW() - (thisDisplay->skin.buttonBorderWidth * 2), input->getDimensions()->getH() - (thisDisplay->skin.buttonBorderWidth * 2),
                      thisDisplay->skin.buttonRadius,
                      TFT_WHITE);
    // Draw the String
    tft.drawString(input->getInput(), 
                   input->getDimensions()->getX() + input->getDimensions()->getW() - thisDisplay->skin.buttonBorderWidth - thisDisplay->skin.buttonPadding,
                   input->getDimensions()->getY() + (input->getDimensions()->getH() / 2));
    input = currentPage->nextInput();
  }
}

uint16_t Display::getFileStateBoarderColor(ElementButton *button)
{
  uint16_t fileTextColor;

  switch (button->getButtonState())
  {
  case ElementButton::STATE::UP:
  case ElementButton::STATE::ROLLOFF:
  case ElementButton::STATE::RELEASED:
    fileTextColor = thisDisplay->skin.fileBorderColor;
    break;
  case ElementButton::STATE::SHORT:
    fileTextColor = thisDisplay->skin.fileShortColor;
    break;
  case ElementButton::STATE::LONG:
    fileTextColor = thisDisplay->skin.fileLongColor;
    break;
  default:
    fileTextColor = Skin::rgb888torgb565(0xFF0000);
    Serial.print(F("Invalid Button STATE: "));
    Serial.print(button->getButtonState());
    Serial.print(F(" for Button : "));
    Serial.println(button->getName());
    break;
  }
  return fileTextColor;
}
uint16_t Display::getFileStateColor(ElementButton *button)
{
  uint16_t fileTextColor;

  switch (button->getButtonState())
  {
  case ElementButton::STATE::UP:
  case ElementButton::STATE::ROLLOFF:
  case ElementButton::STATE::RELEASED:
    fileTextColor = thisDisplay->skin.fileTextColor;
    break;
  case ElementButton::STATE::SHORT:
    fileTextColor = thisDisplay->skin.fileShortColor;
    break;
  case ElementButton::STATE::LONG:
    fileTextColor = thisDisplay->skin.fileLongColor;
    break;
  default:
    fileTextColor = Skin::rgb888torgb565(0xFF0000);
    Serial.print(F("Invalid Button STATE: "));
    Serial.print(button->getButtonState());
    Serial.print(F(" for Button : "));
    Serial.println(button->getName());
    break;
  }
  return fileTextColor;
}
uint16_t Display::getButtonStateColor(ElementButton *button)
{
  uint16_t buttonColor;

  switch (button->getButtonState())
  {
  case ElementButton::STATE::UP:
  case ElementButton::STATE::ROLLOFF:
  case ElementButton::STATE::RELEASED:
    buttonColor = thisDisplay->skin.buttonColor;    
    break;
  case ElementButton::STATE::SHORT:
    buttonColor = thisDisplay->skin.buttonShortColor;
    break;
  case ElementButton::STATE::LONG:
    buttonColor = thisDisplay->skin.buttonLongColor;
    break;
  default:
    buttonColor = Skin::rgb888torgb565(0xFF0000);
    Serial.print(F("Invalid Button STATE: "));
    Serial.print(button->getButtonState());
    Serial.print(F(" for Button : "));
    Serial.println(button->getName());
    break;
  }
  return buttonColor;
}
bool Display::drawButtonIcon(ElementButton *button)
{
  if (!button->getIcon()  == NULL)
  {
    int rc, filecount = 0;
    button->getIcon();
    for (int i = 0; i < sizeof(buffer) / (sizeof(buffer[0])); i++)
    {
      buffer[i] = button->getIcon()[i];
    }

    rc = png.open((const char *)buffer, myOpen, myClose, myRead, mySeek, PNGDraw);
    if (rc == PNG_SUCCESS)
    {

      tft.setCursor(button->getDimensions()->getX() + ((button->getDimensions()->getW() - png.getWidth()) / 2),
                    button->getDimensions()->getY() + ((button->getDimensions()->getH() - png.getHeight()) / 2));

      // Serial.printf("  image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
      rc = png.decode(NULL, 0);
      png.close();
    }
    else
    {
      Serial.print(" Icon open failure: ");
      Serial.println(button->getIcon());
      return false;
    }
    return true;
  }
  return false;
}
void Display::drawFile(ElementPage *currentPage, ElementButton *button)
{
  uint16_t color = getFileStateBoarderColor(button);

  tft.drawRoundRect(button->getDimensions()->getX(), button->getDimensions()->getY(),
                    button->getDimensions()->getW(), button->getDimensions()->getH(),
                    thisDisplay->skin.buttonRadius,
                    color);

  // Draw Icon
  if (!drawButtonIcon(button))
  {
    color = getFileStateColor(button);
    // Draw Name
    tft.setTextSize(thisDisplay->skin.textFontSize);
    tft.setTextColor(color,thisDisplay->skin.fileBackColor,true);
    tft.setTextDatum(TL_DATUM);
    tft.drawString(button->getName(),
                   button->getDimensions()->getX(),
                   button->getDimensions()->getY());
  }
}
void Display::drawButton(ElementPage *currentPage, ElementButton *button)
{
  if (button->isStyleFile())
  {
    drawFile(currentPage, button);
    return;
  }
  if (!button->isHidden())
  {
      tft.setTextSize(thisDisplay->skin.textFontSize);

    uint16_t buttonColor = getButtonStateColor(button);

    // Draw button outline
    tft.fillRoundRect(button->getDimensions()->getX(), button->getDimensions()->getY(),
                      button->getDimensions()->getW(), button->getDimensions()->getH(),
                      thisDisplay->skin.buttonRadius,
                      thisDisplay->skin.buttonBorderColor);
    // Draw button background
    tft.fillRoundRect(button->getDimensions()->getX() + thisDisplay->skin.buttonBorderWidth, button->getDimensions()->getY() + thisDisplay->skin.buttonBorderWidth,
                      button->getDimensions()->getW() - (thisDisplay->skin.buttonBorderWidth * 2), button->getDimensions()->getH() - (thisDisplay->skin.buttonBorderWidth * 2),
                      thisDisplay->skin.buttonRadius,
                      buttonColor);

    // Draw Icon
    if (!drawButtonIcon(button))
    {
      // Draw Name
      tft.setTextColor(thisDisplay->skin.buttonTextColor, buttonColor);
      tft.setTextDatum(CC_DATUM);
      tft.drawString(button->getName(),
                     button->getDimensions()->getX() + (button->getDimensions()->getW() / 2),
                     button->getDimensions()->getY() + (button->getDimensions()->getH() / 2));
    }
  }
}
void Display::drawButtons(ElementPage *currentPage)
{
//  tft.setTextColor(thisDisplay->skin.buttonTextColor, thisDisplay->skin.buttonColor);
  // tft.setTextSize(thisDisplay->skin.textFontSize);
//  tft.setTextDatum(CC_DATUM);

  // Setup iterator & loop on buttons
  ElementButton *button = currentPage->nextButton();

  while (button)
  {
    drawButton(currentPage, button);

    // Get next button
    button = currentPage->nextButton();
  }
}

void Display::drawRectangle(ElementPage *currentPage)
{ // Can be used for Info box
  /*
    // Button
    this->buttonTextColor = TBA_ORANGE;
    this->buttonColor = TBA_PURPLE;
    this->buttonShortColor = TBA_SHORT;
    this->buttonLongColor = TBA_LONG;
    this->buttonBorderColor = TBA_GRAY;
    // File
    this->fileTextColor = TBA_PURPLE;
    this->fileBackColor = TBA_GRAY;
    this->fileShortColor = TBA_SHORT;
    this->fileLongColor = TBA_LONG;
    this->fileBorderColor = TBA_GRAY;
    // Button & File
    this->buttonMargin = 5;
    this->buttonBorderWidth = 3;
    this->buttonPadding = 3;
    this->buttonRadius = 3;

    // Rectangle
    this->rectangleBackColor = TBA_NOT_WHITE;
    this->rectangleBorderColor = TBA_GRAY;
    this->rectangleRadius = 0;
  */

  // iterator
  ElementRectangle *rectangle = currentPage->nextRectangle();
  while (rectangle)
  {
    tft.fillRoundRect(rectangle->getDimensions()->getX(), rectangle->getDimensions()->getY(),
                      rectangle->getDimensions()->getW(), rectangle->getDimensions()->getH(),
                      thisDisplay->skin.rectangleRadius, thisDisplay->skin.rectangleBorderColor);

    uint8_t offset = thisDisplay->skin.buttonBorderWidth;
    tft.fillRoundRect(rectangle->getDimensions()->getX() +  offset,      rectangle->getDimensions()->getY() +  offset,
                      rectangle->getDimensions()->getW() - (offset * 2), rectangle->getDimensions()->getH() - (offset * 2),
                      thisDisplay->skin.rectangleRadius, thisDisplay->skin.rectangleBackColor);

    rectangle = currentPage->nextRectangle();
  }

  /*
  // Would like a transparent backgroud, but way to slow, is there a faster way???
  // screen overlay
  for (uint16_t x = 0; x < 3; x++)
  {
    tft.drawRoundRect(25 + x, 125 + x, thisDisplay->skin.getScreenWidth() - (25 * 2) - (x * 2), thisDisplay->skin.getScreenHeight() - 125 - (25 * 2) - (x * 2), 3, TBA_GRAY);
  }

  for (uint16_t y = 125+5; y < thisDisplay->skin.getScreenHeight() - (25 * 2)-5; y++)
    for (uint16_t x = 25+5; x < thisDisplay->skin.getScreenWidth() - 25-5; x++)
    {
      // tft.drawPixel(x, y, TBA_GRAY,128, 128, tft.readPixel(x, y));
      tft.drawPixel(x, y, TBA_GRAY, 128);
      tft.drawPixel(x, y, TBA_GRAY);
    }
  */
}

void Display::drawCurrentPage(ElementPage *currentPage)
{
  // Reset Iterators
  currentPage->resetIterators();

  // Clear the screen if required
  if (currentPage->getClearScreen())
  { // Clear the screen
    tft.fillScreen(thisDisplay->skin.textBackgroundColor);
  }

  // Display Header on the screen if required
  thisDisplay->drawHeader(currentPage);

  // Display Rectangle
  thisDisplay->drawRectangle(currentPage);

  // Draw Label
  thisDisplay->drawLabel(currentPage);

  // Draw variable
  thisDisplay->drawVariable(currentPage);

  // Draw Input
  thisDisplay->drawInput(currentPage);

  // Draw Buttons
  thisDisplay->drawButtons(currentPage);
}

Point *Display::getScreenTouchPoint(bool &pressed)
{
  pressed = tft.getTouch(&t_x, &t_y);
  return new Point(t_x, t_y);

  // By default, x & y retain the last position after release.
  // if (pressed)
  // point = new Point(t_x, t_y);
  // else
  // point = new Point();
  // return point;
}

void Display::debugSerial(const char * debugLocation)
{
  Serial.print(" <");
  Serial.print(__FILENAME__);
  Serial.print("> ");


  Serial.print(F(" thisDisplay '0x"));
  Serial.print((unsigned int)(thisDisplay), HEX);

  // // Serial.print(" H W: '");
  // // Serial.print (thisDisplay->skin.getHeaderWidth());

  Serial.print(F("' debug Loc: '"));
  Serial.print(debugLocation);
  Serial.print(F("' "));
}

TFT_eSPI Display::getScreenTFT()
{
  return tft;
}

Dimensions *Display::getScreenDimensions()
{
  return new Dimensions(0, 0, skin.getScreenWidth(), skin.getScreenHeight());
}

#endif // Display