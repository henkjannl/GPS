#pragma once

#include "gui_elements.h"

class ScreenGpsValues {

  public:
    ScreenGpsValues(const TFT_eSPI& tft);
    void drawFirstTime();
    void drawUpdate();

    LabelAndValue Status;
    LabelAndValue Lattitude;
    LabelAndValue Longitude;
    LabelAndValue Date;
    LabelAndValue Time;
    LabelAndValue Speed;
    LabelAndValue Course;
    LabelAndValue Altitude;
    LabelAndValue Sattelites;
    LabelAndValue Accuracy;

    //TFT_eSPI_Button btnOK;
    Button btnOK;

  private:
    TFT_eSPI *_tft;
    std::vector<LabelAndValue*> labels;
};
