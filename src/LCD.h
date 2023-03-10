#pragma once

#ifndef LCD_h
#define LCD_h

#include "Arduino.h"
#include <mutex>
#include <thread>

/* The SPIFFS (FLASH file system) is used
   to hold touch screen calibration data
*/
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "GlobalTFT.h"

#include "TBA_FileSystem.h"

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
    Skin::ROTATE rotate;

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
    enum SCREEN_CAPTURE_STATE
    { // Do not change order.
        ERROR = -1,
        WAITING,
        CAPTURE,
        COPING
    };

    LCD(const char *name, Skin::ROTATE rotate) : name(name), rotate(rotate)
    {
        // this->name = name;
        // this->rotate = rotate;
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
    static LCD *Initialize(const char *name, Skin::ROTATE rotate);
    static LCD *GetInstance();

    /** Other static methods should be defined outside the class. */

    Point *getScreenTouchPoint(boolean &pressed);

    const char *getName();

    void screenImage(const char *filename);
    void saveImage(const char *filename);

    void dumpFS(fs::FS &fs, const char *where, const char *dirname, uint8_t levels);
    void copyFile(const char *filename);
    void sdINIT();

    void debugSerial(const char *debugLocation);

    void writeBitmap(File &file, int w, int h);
    void removeBMP(fs::FS &fs);
};

LCD *LCD::thisLCD{nullptr};
std::mutex LCD::mutex_;

void LCD::writeBitmap(File &file, int w, int h)
{
    size_t rowSize = 4 * ((3 * w + 3) / 4); // padded to multiple of 4
    size_t fileSize = 54 + h * rowSize;     // includes header

    // Write image header.
    uint8_t header[54] = {
        // File header.
        'B', 'M',
        (uint8_t)(fileSize >> 0),
        (uint8_t)(fileSize >> 8),
        (uint8_t)(fileSize >> 16),
        (uint8_t)(fileSize >> 24),
        0, 0, 0, 0, 54, 0, 0, 0,

        // Image info header.
        40, 0, 0, 0,
        (uint8_t)(w >> 0),
        (uint8_t)(w >> 8),
        (uint8_t)(w >> 16),
        (uint8_t)(w >> 24),
        (uint8_t)(h >> 0),
        (uint8_t)(h >> 8),
        (uint8_t)(h >> 16),
        (uint8_t)(h >> 24),
        1, 0, 24, 0};
    file.write(header, sizeof header);

    // Write image data.
    uint8_t row[rowSize] = {0};
    uint8_t color[3] = {0, 0, 0}; // RGB and 565 format color buffer for N pixels
    char deleteFile[256];
char *per="%";
    for (int y = h; y > 0; y--)
    {
        if (y % 24 == 0)
        {
            double percent = (1 - ((double)y / (double)h)) * 100;
            sprintf(deleteFile, " %3.1f %s complete", percent,per);
            Serial.println(deleteFile);
        }
        for (int x = 0; x < w; x++)
        {
            tft.readRectRGB(x, y, 1, 1, color);
            row[3 * x + 0] = (uint16_t)color[2];
            row[3 * x + 1] = (uint16_t)color[1];
            row[3 * x + 2] = (uint16_t)color[0];
        }
        file.write(row, sizeof row);
    }
}

void LCD::touch_calibrate_setup()
{
    // Set communication speed
    tft.begin(4000000);

    // Initialise the TFT screen
    tft.init();

    tft.setRotation(this->rotate);
    switch (this->rotate)
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

        // Store Calibration Data
        File f = SPIFFS.open(calibrationFile, "w");
        if (f)
        {
            f.write((const unsigned char *)calData, 14);
            f.close();
        }
    }
}

void LCD::removeBMP(fs::FS &fs)
{
    Serial.println("** Look for BMP **");

    File root = fs.open("/");
    if (!root)
    {
        Serial.print("** Failed to open directory to delete BMP ** '/'");
        return;
    }
    File file = root.openNextFile();
    while (file)
    {
        if (!file.isDirectory())
        {
            char *compName = (char *)file.name();
            int fileName_len = strlen(compName);
            compName = (char *)compName + fileName_len - 4;
            if (fileName_len >= 4 && strcmp(compName, ".bmp") == 0)
            {

                Serial.print("FILE: ");
                Serial.print(file.name());
                Serial.print("  SIZE: ");
                Serial.print(file.size());

                Serial.print(" fileName_len: ");
                Serial.print(fileName_len);
                Serial.print(" compName: ");
                Serial.print(compName);

                // char *deleteFile[strlen(file.name()) + 2];
                // snprintf_P(deleteFile, sizeof(deleteFile), PSTR("/%s"), file.name());

                char deleteFile[256];
                sprintf(deleteFile, "/%s", file.name());

                // memcpy ( ((char *)deleteFile + 1), file.name(), strlen(file.name()) + 2 );
                // deleteFile[0] = (char*)'/';

                Serial.print(" deleteFile: '");
                Serial.print((const char *)deleteFile);
                Serial.println("'");

                Serial.print(" *** *** Delete FILE");

                fs.remove((const char *)deleteFile);
            }
        }
        file = root.openNextFile();
    }
    root.close(); //???????????????????
}

/** The first time we call GetInstance we will lock the storage location
 *  and then we make sure again that the variable is null and then we
 *  set the name and rotate. */
LCD *LCD::Initialize(const char *name, Skin::ROTATE rotate)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (thisLCD == nullptr)
    {
        TBA_FileSystem fileSystem = TBA_FileSystem(); // init SPIFFS & SD
        thisLCD = new LCD(name, rotate);
        thisLCD->touch_calibrate_setup();
    }

    thisLCD->removeBMP(SPIFFS);
    thisLCD->removeBMP(SD);

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

const char *LCD::getName()
{
    return this->name;
}

// https://cdn.hackaday.io/files/274271173436768/Simplified%20Windows%20BMP%20Bitmap%20File%20Format%20Specification.htm
/*
Offset Size Decimal value (0-255)   Description
0      2    -   66   77    -          "BM"
2      4    N   N>>8 N>>16 N>>24      size, N, of a BMP file
6      2    -   0    0     -          application specific - not used by the yBmp namespace
8      2    -   0    0     -          application specific - not used by the yBmp namespace
10     4    54  0    0     0          number of bytes from beginning of file to beginning of pixel data
14     4    40  0    0     0          number of bytes from this point to the beginning of pixel data
18     4    w   w>>8 0     0          Width, w, of image (in pixels, limited to 216-1=65,535)
22     4    h   h>>8 0     0          Height, h, of image (in pixels, limited to 216-1=65,535). Height values can be positive or negative. Negative values imply that the image is mirrored, top to bottom
26     2    -   1    0     -          number of color planes used
28     2    -   24   0     -          number of bits per pixel
30     4    0   0    0     0          no compression
34     4    n   n>>8 n>>16 n>>24      size, n, of image (n=h*(3*w+w%4)=N-54)
38     4    19  11   0     0          horizontal resolution of image (2835 pixels/meter)
42     4    19  11   0     0          vertical resolution of image (2835 pixels/meter)
46     4    0   0    0     0          number of colors in the palett
*/

void LCD::copyFile(const char *fileName)
{
    File sourceFile = SPIFFS.open(fileName);
    File destFile = SD.open(fileName, FILE_WRITE);
    static uint8_t buf[bufSize];
    Serial.print("SPIFFS 2 SD -> progress: ");
    Serial.println(fileName);

    boolean showDebug = false;
    while (sourceFile.available() > 0)
    {
        int bytesAvailable = sourceFile.available();
        if (showDebug)
        {
            Serial.print("BytesAvailable A: ");
            Serial.print(bytesAvailable);
        }
        else
        {
            Serial.print(".");
        }

        if (bytesAvailable > bufSize)
        {
            bytesAvailable = bufSize;
        }

        sourceFile.read(buf, bytesAvailable);

        if (showDebug)
        {
            Serial.print(" -> BytesAvailable B: ");
            Serial.println(bytesAvailable);
        }
        destFile.write(buf, bytesAvailable);
    }

    sourceFile.close();
    destFile.flush();
    destFile.close();
    Serial.println("");
    Serial.println("copy done");
}

void LCD::sdINIT()
{
    //    SD.end();
    if (SD.begin())
    {
        Serial.println("SD Mounted");
    }
    else
    {
        Serial.println("SD Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();

    String stgCardType = "";

    switch (cardType)
    {
    case CARD_NONE:
        stgCardType = "No SD card attached";
        return;
    case CARD_MMC:
        stgCardType = "MMC";
        break;
    case CARD_SD:
        stgCardType = "SD";
        break;
    case CARD_SDHC:
        stgCardType = "SDHC";
        break;
    default:
        stgCardType = "-- Unknown --";
    }

    Serial.printf("SD Card Type: %s\n", stgCardType);
}

void LCD::dumpFS(fs::FS &fs, const char *where, const char *dirname, uint8_t levels)
{
    Serial.println("\n====================================================================");
    Serial.printf("Listing directory: where '%s' : dir '%s'\n", where, dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.print("** Failed to open directory ** ");
        Serial.println(root.name());
        return;
    }
    if (!root.isDirectory())
    {
        Serial.print("** Not a directory ** ");
        Serial.println(root.name());
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                dumpFS(fs, where, file.name(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
    file.close(); //???????????????????
}

void LCD::saveImage(const char *filename)
{

    File bmpFile = SPIFFS.open(filename, FILE_WRITE);
    thisLCD->writeBitmap(bmpFile, 320, 240);
    bmpFile.flush();
    bmpFile.close();
}

void LCD::screenImage(const char *filename)
{
    char nameBuffer[256];
    sprintf(nameBuffer, "/%s.bmp", filename);

    for (uint8_t i = 1; i < nameBuffer[i] != '\0'; i++)
    {
        if (nameBuffer[i] == ' ')
        {
            nameBuffer[i] = '_';
        }
        if (nameBuffer[i] == '/')
        {
            nameBuffer[i] = '_';
        }
    }

    if (SD.exists(nameBuffer))
    { // already copied from SPIFFS to SD
        Serial.print(".");
    }
    else if (SPIFFS.exists(nameBuffer))
    { // copy from SPIFFS to SD
        Serial.printf("Capture Image: '%s'\n", nameBuffer);
        copyFile(nameBuffer);
    }
    else
    { // Create the image on SPIFFS
        Serial.printf("Coping File: '%s'\n", nameBuffer);
        saveImage(nameBuffer);
    }
}

void LCD::debugSerial(const char *debugLocation)
{
    Serial.print(" <");
    Serial.print(__FILENAME__);
    Serial.print("> ");

    Serial.print(F(" thisLCD '0x"));
    Serial.print((unsigned int)(thisLCD), HEX);

    Serial.print(F("' debug Loc: '"));
    Serial.print(debugLocation);
    Serial.print(F("' "));
}

#endif // LCD