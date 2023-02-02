/*
  DisplayICON.h - Library for DisplayICON code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.
*/

#pragma once

#ifndef DisplayICON_h
#define DisplayICON_h

#include "GlobalConst.h"

// A simple image slideshow which reads all the .JPG files from the root
// directory of a SD card and shows each for 1 second on an ILI9341 display.

#include <PNGdec.h>
#include <SD.h>
// Include the TFT library https://github.com/Bodmer/TFT_eSPI
#include "SPI.h"
#include <TFT_eSPI.h> // Hardware-specific library

PNG png;

// Functions to access a file on the SD card
File myfile;

void *myOpen(const char *filename, int32_t *size)
{
  myfile = SD.open(filename);
  *size = myfile.size();
  return &myfile;
}
void myClose(void *handle)
{
  if (myfile)
    myfile.close();
}
int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length)
{
  if (!myfile)
    return 0;
  return myfile.read(buffer, length);
}
int32_t mySeek(PNGFILE *handle, int32_t position)
{
  if (!myfile)
    return 0;
  return myfile.seek(position);
}
// Function to draw pixels to the display
void PNGDraw(PNGDRAW *pDraw)
{
  uint16_t usPixels[320];
  png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(tft.getCursorX(), tft.getCursorY() + pDraw->y, pDraw->iWidth, 1, usPixels, 0x7E0);//uint16_t transparent_color = Skin::rgb888torgb565(0x00FF00); // 0x7E0; // green
}

#endif
