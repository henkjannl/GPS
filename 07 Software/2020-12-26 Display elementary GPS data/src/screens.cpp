#include "screens.h"

ScreenGpsValues::ScreenGpsValues(TFT_eSPI& tft, Theme& theme) {
  _tft = (TFT_eSPI *) &tft;
  _theme = &theme;

  int top=5;

  Lattitude.  init(tft, theme, "Lattitude",   88, 136, top); top+=25;
  Longitude.  init(tft, theme, "Longitude",   88, 136, top); top+=25;
  Date.       init(tft, theme, "Date",        88, 136, top); top+=25;
  Time.       init(tft, theme, "Time",        88, 136, top); top+=25;
  Speed.      init(tft, theme, "Speed",       88, 136, top); top+=25;
  Course.     init(tft, theme, "Course",      88, 136, top); top+=25;
  Altitude.   init(tft, theme, "Altitude",    88, 136, top); top+=25;
  Sattelites. init(tft, theme, "Sattelites",  88, 136, top); top+=25;
  Accuracy.   init(tft, theme, "Accuracy",    88, 136, top); top+=25;
  Accuracy.alignLeft();
  Status.     init(tft, theme, "Free heap",   88, 136, top); top+=25;
  Status.alignLeft();

  btnOK.init(tft, theme, "OK", 88, 136, top); top+=25;

  labels.push_back(&Status    );
  labels.push_back(&Lattitude );
  labels.push_back(&Longitude );
  labels.push_back(&Date      );
  labels.push_back(&Time      );
  labels.push_back(&Speed     );
  labels.push_back(&Course    );
  labels.push_back(&Altitude  );
  labels.push_back(&Sattelites);
  labels.push_back(&Accuracy  );

  // ToDo: Also add button
}

void ScreenGpsValues::drawFirstTime() {
  _tft->fillScreen(TFT_BLACK);

  for(auto label:labels) {
    label->drawFirstTime();
  }

  btnOK.drawFirstTime();
}

void ScreenGpsValues::drawUpdate() {

  for(auto label:labels) {
    label->drawUpdate();
  }

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  boolean pressed = _tft->getTouch(&t_x, &t_y); // Reverse coordinates due to portrait mode
  //t_y=320-t_y;

  _tft->drawFastVLine(t_x, t_y-5, 11, TFT_WHITE);
  _tft->drawFastHLine(t_x-5, t_y, 11, TFT_WHITE);

  // Check if any key coordinate boxes contain the touch coordinates
  if (pressed) {
    btnOK.handleClick(t_x, t_y); // If there are more buttons, the result may not be ignored
  } else {
    btnOK.release();
  }

  btnOK.drawUpdate();
}
