#pragma once

#include "TFT_eSPI.h"
#include <string>     
#include <vector> 

enum guiAlign { GA_LEFT, GA_CENTER, GA_RIGHT };

// Theme to provide colors for the user interface, stored as single global variable
struct Theme {
  uint32_t windowBackground          = 0x0000;   // 00, 00, 00
  uint32_t labelBackground           = 0x4208;   // 40, 40, 40
  uint32_t labelTitleText            = 0xF800;   // FF, 00, 00
  uint32_t labelValueText            = 0xFFFF;   // FF, FF, FF
  uint32_t buttonUnclickedBackground = 0x8410;   // 80, 80, 80
  uint32_t buttonUnclickedText       = 0xFFFF;   // FF, FF, FF
  uint32_t buttonClickedBackground   = 0xA000;   // A0, 00, 00
  uint32_t buttonClickedText         = 0xFFFF;   // FF, FF, FF
};

// A static box with a value
class GUI_Label {

  public:
    void init(TFT_eSPI& tft, Theme& theme, const String& label, int left, int width, int top, int height=20);
    void drawFirstTime();
    void drawUpdate() {}; 
    void alignLeft()   { _align=GA_LEFT;   };
    void alignCenter() { _align=GA_CENTER; };
    void alignTight()  { _align=GA_RIGHT;  };

  protected:
    TFT_eSPI *_tft;
    Theme *_theme;
    guiAlign _align=GA_CENTER;
    int _left, _width, _top, _height;
    String _label;
};

class GUI_LabelAndValue : public GUI_Label {

  public:
    void init(TFT_eSPI& tft, Theme& theme, const String& label, int left, int width, int top, int height=20);
    void drawFirstTime();
    void drawUpdate();
    void updateValue(const String& string);
    void alignLeft() { _align=GA_LEFT; };

  protected:
    String _newValue;
    String _prevValue;
};

class GUI_Button : public GUI_Label {
  public:
    void drawFirstTime();
    void drawUpdate();

    bool handleClick(int16_t x, int16_t y); // Check if click is inside button and handle state
    void release()     { _lastState=_currState; _currState=false; };
    bool justPressed() { return _currState && !_lastState; };
    bool isPressed()   { return _currState; };

  protected:
    bool  _currState, _lastState; // Button states
};
