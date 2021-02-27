#pragma once

#include "gui_elements.h"

class ScreenGpsValues {

  public:
    ScreenGpsValues(TFT_eSPI& tft, Theme& theme);
    void drawFirstTime();
    void drawUpdate();

    GUI_LabelAndValue Status;
    GUI_LabelAndValue Lattitude;
    GUI_LabelAndValue Longitude;
    GUI_LabelAndValue Date;
    GUI_LabelAndValue Time;
    GUI_LabelAndValue Speed;
    GUI_LabelAndValue Course;
    GUI_LabelAndValue Altitude;
    GUI_LabelAndValue Sattelites;
    GUI_LabelAndValue Accuracy;

    GUI_Button btnOK;

  protected:
    TFT_eSPI *_tft;
    Theme *_theme;
    std::vector<GUI_LabelAndValue*> labels;
};
