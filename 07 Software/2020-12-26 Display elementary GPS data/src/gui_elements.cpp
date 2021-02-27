#include "gui_elements.h"

void GUI_Label::init(TFT_eSPI& tft, Theme& theme, const String& label, int left, int width, int top, int height) {
  _tft     = (TFT_eSPI *) &tft;
  _theme   = &theme;
  _label   = String(label);
  _left    = left;
  _width   = width;
  _top     = top;
  _height  = height;
}

void GUI_Label::drawFirstTime() {
  _tft->fillRoundRect(_left, _top, _width, _height, 7, _theme->labelBackground);
  _tft->setTextFont(2);
  _tft->setTextColor(_theme->labelValueText, _theme->labelBackground);
  _tft->setTextDatum(MR_DATUM);
  _tft->drawString(_label, _left-3, _top+(_height>>1));
}

void GUI_LabelAndValue::init(TFT_eSPI& tft, Theme& theme, const String& label, int left, int width, int top, int height) {
  GUI_LabelAndValue::init(tft, theme, label, left, width, top, height);
  _prevValue= String("@");
  _newValue = String("");
}

void GUI_LabelAndValue::drawFirstTime() {
  _tft->fillRoundRect(_left, _top, _width, _height, 7, _theme->labelBackground);
  _tft->setTextFont(2);
  _tft->setTextColor(_theme->labelValueText, _theme->labelBackground);
  _tft->setTextDatum(MR_DATUM);  
  _tft->drawString(_label, _left-4, _top+(_height >> 1));
}

void GUI_LabelAndValue::updateValue(const String& value) {
  _newValue=String(value);
  drawUpdate();
}

void GUI_LabelAndValue::drawUpdate() {
  if(_newValue != _prevValue) {
    _tft->fillRoundRect(_left, _top, _width, _height, 7, _theme->labelBackground);
    _tft->setTextFont(2);
    _tft->setTextColor(_theme->labelValueText, _theme->labelBackground);

    switch(_align) {
      case GA_LEFT:
        _tft->setTextDatum(ML_DATUM);  
        _tft->drawString(_newValue, _left+4, _top+(_height >> 1));
        break;

      case GA_CENTER:
        _tft->setTextDatum(MC_DATUM);  
        _tft->drawString(_newValue, _left+(_width >> 1), _top+(_height >> 1));
        break;

      case GA_RIGHT:
        _tft->setTextDatum(MR_DATUM);  
        _tft->drawString(_newValue, _left+_width-4, _top+(_height >> 1));
        break;
    }

    _prevValue=String(_newValue);
  }
}

void GUI_Button::drawFirstTime() {
  _tft->setTextFont(2);
  _tft->fillRoundRect(_left, _top, _width, _height, 7, _theme->buttonUnclickedBackground);
  _tft->setTextColor(_theme->buttonUnclickedText,_theme->buttonUnclickedBackground);
  _tft->setTextDatum(MC_DATUM);
  _tft->drawString(_label, _left+(_width>>1), _top+(_height>>1));
}

void GUI_Button::drawUpdate() {
  _tft->fillRoundRect(_left, _top, _width, _height, 7, 
      _currState ? _theme->buttonUnclickedText : _theme->buttonUnclickedBackground);
  _tft->setTextFont(2);
  _tft->setTextColor(_currState ? _theme->buttonClickedText   : _theme->buttonClickedBackground, 
                     _currState ? _theme->buttonUnclickedText : _theme->buttonUnclickedBackground);
  _tft->setTextDatum(MC_DATUM);
  _tft->drawString(_label, _left+(_width >>1), _top+(_height>>1));
}

bool GUI_Button::handleClick(int16_t x, int16_t y) {
  _lastState = _currState;
  _currState = ( (x >= _left) && (x < (_left + _width)) &&
                 (y >= _top ) && (y < (_top + _height)));

  return _currState;
}
