#pragma once

#include "TFT_eSPI.h"
#include <string>     
#include <vector> 

enum guiAlign { GA_LEFT, GA_CENTER, GA_RIGHT };

// Base class for GUI elements
class GuiElement {

  public:
    void init(const TFT_eSPI& tft, const String& label, int left, int top, int width, int height=20);
    virtual void drawFirstTime();
    virtual void drawUpdate();
    void alignLeft()   { _align=GA_LEFT;   };
    void alignCenter() { _align=GA_CENTER; };
    void alignRight()  { _align=GA_RIGHT;  };

  protected:
    TFT_eSPI *_tft;
    guiAlign _align=GA_CENTER;
    int _left, _width, _top, _height;
    String _label;
};

class LabelAndValue : public GuiElement {

  public:
    void drawFirstTime();
    void drawUpdate();
    void updateValue(const String& string);

  protected:
    String _newValue="";
    String _prevValue="@"; // Ensure it is drawn the first time
};

class Button : public GuiElement {

  public:
    void drawFirstTime();
    void drawUpdate();
    void updateValue(bool clicked) { _newState=clicked; }
    bool contains(int16_t x, int16_t y);
    bool isPressed()    { return _newState; }
    bool justPressed()  { return (_newState && !_prevState); }
    bool justReleased() { return (!_newState && _prevState); }

  protected:
    bool _newState = false;
    bool _prevState = true; // Ensure it is drawn the first time
};
