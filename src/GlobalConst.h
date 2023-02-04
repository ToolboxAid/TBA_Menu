/*
  GlobalConst.h - Library for GlobalConst code.
  Created by Mr Q, December 2, 2022.
  Released into the public domain.

  Global and Constant variables.
*/

#pragma once

#ifndef GlobalConst_h
#define GlobalConst_h

#include "Arduino.h"

const char * TAG = "v1.1.35-c";

#include <TFT_eSPI.h>       // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();  // Instance library

char buffer[256];

#endif