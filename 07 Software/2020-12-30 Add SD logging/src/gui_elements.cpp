#include "gui_elements.h"

// ToDo: implement color themes
const int32_t CLR_DARK       = 0x2104;   // 20, 20, 20
const int32_t CLR_PUSHEDBTN  = 0xA269;   // A6, 4C, 4C

void GuiElement::init(const TFT_eSPI& tft, const String& label, int left, int top, int width, int height) {
  _tft     = (TFT_eSPI *) &tft;
  _label   = String(label);
  _left    = left;
  _width   = width;
  _top     = top;
  _height  = height;
}

void LabelAndValue::drawFirstTime() {
  _tft->setTextFont(2);
  _tft->setTextColor(TFT_RED, TFT_BLACK);
  _tft->setTextDatum(MR_DATUM);
  _tft->drawString(_label, _left-3, _top+(_height>>1));
  _tft->fillRoundRect(_left, _top, _width, _height, 7, CLR_DARK);
  _prevValue= String("");
}

void LabelAndValue::updateValue(const String& value) {
  _newValue=String(value);
}

void LabelAndValue::drawUpdate() {
  if(_newValue != _prevValue) {
    _tft->fillRoundRect(_left, _top, _width, _height, 7, CLR_DARK);
    _tft->setTextFont(2);
    _tft->setTextDatum((_align==GA_RIGHT) ? MR_DATUM : ML_DATUM);

    _tft->setTextColor(TFT_WHITE, CLR_DARK);
    _tft->drawString(_newValue, (_align==GA_RIGHT) ? _left+_width-4 : _left+4, _top+(_height >> 1));
    _prevValue=String(_newValue);
  }
}

void Button::drawFirstTime() { 
  // Nothing to do here
}

void Button::drawUpdate() {
  if(_newState != _prevState) {
    int32_t clr = _newState ? CLR_DARK : CLR_PUSHEDBTN;
    _tft->fillRoundRect(_left, _top, _width, _height, 7, clr);
    _tft->setTextFont(2);
    _tft->setTextColor(TFT_WHITE, clr);
    switch (_align) {
      case GA_LEFT:
        _tft->setTextDatum(ML_DATUM);    
        _tft->drawString(_label, _left+4, _top+(_height >> 1));
        break;

      case GA_CENTER:
        _tft->setTextDatum(MC_DATUM);    
        _tft->drawString(_label, _left+(_width >> 1), _top+(_height >> 1));
        break;

      case GA_RIGHT:
        _tft->setTextDatum(MR_DATUM);    
        _tft->drawString(_label, _left+_width-4, _top+(_height >> 1));
        break;
      }

    _prevState=_newState;
  }
}

bool Button::contains(int16_t x, int16_t y) {
  return ((x >= _left) && (x < (_left + _width)) &&
          (y >= _top) && (y < (_top + _height)));
}
