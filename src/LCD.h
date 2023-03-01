#pragma once

#ifndef LCD_h
#define LCD_h

#include "Arduino.h"
#include <mutex>
#include <thread>

#include "GlobalConst.h"

/* The SPIFFS (FLASH file system) is used
   to hold touch screen calibration data
*/
#include "FS.h"
#include <SPI.h>

#include "TBA_FileSystem.h"

// #include "ElementPage.h"
#include "Skin.h"
#include "Point.h"

/* Set REPEAT_CAL to true instead of false to run calibration
   again, otherwise it will only be done once.
   Repeat calibration if you change the screen rotation.
   */
#define REPEAT_CAL false // Rotation Change, set to true to update file

/**
 * The LCD class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class LCD
{
    /**
     * The LCD's constructor/destructor should always be private to
     * prevent direct construction/desctruction calls with the `new`/`delete`
     * operator.
     */
private:
    static LCD *thisLCD;
    static std::mutex mutex_;

    const char *name;
    Skin *skin;

    /* This is the file name used to store the calibration data
       You can change this to create new calibration files.
       The SPIFFS file name must start with "/".
    */
    inline static const char *CALIBRATION_FILE_LEFT = "/TouchCal.Left.Data";
    inline static const char *CALIBRATION_FILE_RIGHT = "/TouchCal.Right.Data";
    inline static const char *CALIBRATION_FILE_UP = "/TouchCal.Up.Data";
    inline static const char *CALIBRATION_FILE_DOWN = "/TouchCal.Down.Data";
    inline static const char *CALIBRATION_FILE_REV0 = "/TouchCal.R0.Data";
    inline static const char *CALIBRATION_FILE_REV1 = "/TouchCal.R1.Data";
    inline static const char *CALIBRATION_FILE_REV2 = "/TouchCal.R2.Data";
    inline static const char *CALIBRATION_FILE_REV3 = "/TouchCal.R3.Data";

    void touch_calibrate_setup();
    void touch_calibrate(const char *calibrationFile);

protected:
    LCD(const char *name, Skin *skin) : name(name), skin(skin)
    {
        // this->name = name;
        // this->skin = skin;
    }
    ~LCD() {}

public:
    /** LCDs should not be cloneable. */
    LCD(LCD &other) = delete;
    /** LCDs should not be assignable. */
    void operator=(const LCD &) = delete;

    /** These static method(s) that controls the access to the singleton
     *  instance. On the first run, it creates a singleton object and places it
     *  into the static field. On subsequent runs, it returns the client existing
     *  object stored in the static field.
     *  You must Initialize prior to GetInstance (will display error if out of order)
     *  */
    static LCD *Initialize(const char *name, Skin *skin);
    static LCD *GetInstance();

    /** Other static methods should be defined outside the class. */

    Point *getScreenTouchPoint(boolean &pressed);
    void debugSerial(const char *debugLocation);

    const char *getName()
    {
        return this->name;
    }
    Skin *getSkin()
    {
        return this->skin;
    }
};

LCD *LCD::thisLCD{nullptr};
std::mutex LCD::mutex_;

void LCD::touch_calibrate_setup()
{
    // Set communication speed
    tft.begin(4000000);

    // Initialise the TFT screen
    tft.init();
    tft.setRotation(this->skin->getRotation());

    // Calibrate the touch screen and determin the scaling factors
    switch (this->skin->getRotation())
    {
    case Skin::ROTATE::SD_UP:
        touch_calibrate(CALIBRATION_FILE_UP);
        break;
    case Skin::ROTATE::SD_LEFT:
        touch_calibrate(CALIBRATION_FILE_LEFT);
        break;
    case Skin::ROTATE::SD_DOWN:
        touch_calibrate(CALIBRATION_FILE_DOWN);
        break;
    case Skin::ROTATE::SD_RIGHT:
        touch_calibrate(CALIBRATION_FILE_RIGHT);
        break;
    case Skin::ROTATE::Reverse0:
        touch_calibrate(CALIBRATION_FILE_REV0);
        break;
    case Skin::ROTATE::Reverse1:
        touch_calibrate(CALIBRATION_FILE_REV1);
        break;
    case Skin::ROTATE::Reverse2:
        touch_calibrate(CALIBRATION_FILE_REV2);
        break;
    case Skin::ROTATE::Reverse3:
        touch_calibrate(CALIBRATION_FILE_REV3);
        break;
    }

    tft.setTextFont(0);
}

void LCD::touch_calibrate(const char *calibrationFile)
{
    uint16_t calData[5];
    uint8_t calDataOK = 0;

    // check if calibration file exists and size is correct
    if (SPIFFS.exists(calibrationFile))
    {
        if (REPEAT_CAL)
        { // Delete if we want to re-calibrate
            SPIFFS.remove(calibrationFile);
        }
        else
        {
            File f = SPIFFS.open(calibrationFile, "r");
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
        File f = SPIFFS.open(calibrationFile, "w");
        if (f)
        {
            f.write((const unsigned char *)calData, 14);
            f.close();
        }
    }
}

/** The first time we call GetInstance we will lock the storage location
 *  and then we make sure again that the variable is null and then we
 *  set the name and skin. */
LCD *LCD::Initialize(const char *name, Skin *skin)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (thisLCD == nullptr)
    {
        TBA_FileSystem fileSystem = TBA_FileSystem(); // init SPIFFS & SD
        thisLCD = new LCD(name, skin);
        thisLCD->touch_calibrate_setup();
    }
    return thisLCD;
}

/* It must be initialized first */
LCD *LCD::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    while (thisLCD == nullptr)
    {
        Serial.println("You must first Initialize LCD prior to getting an instance");
        delay(1000);
    }
    return thisLCD;
}

uint16_t pointX = 0, pointY = 0; // Store the touch coordinates
Point *LCD::getScreenTouchPoint(boolean &pressed)
{
    pressed = tft.getTouch(&pointX, &pointY);
    return new Point(pointX, pointY);

    //// By default, x & y retain the last position after release.
    //// If this is not the result you desire, you have two options
    //// 1) move pointX & pointY inside this method
    //// 2) uncomment the code below & comment out above

    // if (pressed)
    // point = new Point(pointX, pointY);
    // else
    // point = new Point();
    // return point;
}

void LCD::debugSerial(const char *debugLocation)
{
    Serial.print(" <");
    Serial.print(__FILENAME__);
    Serial.print("> ");

    Serial.print(F(" thisLCD '0x"));
    Serial.print((unsigned int)(thisLCD), HEX);

    // // Serial.print(" H W: '");
    // // Serial.print (this->thisLCD->skin->getHeaderWidth());

    Serial.print(F("' debug Loc: '"));
    Serial.print(debugLocation);
    Serial.print(F("' "));
}

#endif // LCD