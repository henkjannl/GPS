#include "screens.h"

ScreenGpsValues::ScreenGpsValues(const TFT_eSPI& tft) {
  int top=5;
  _tft = (TFT_eSPI *) &tft;
  Lattitude.  init(tft, "Lattitude",   88, top, 136); top+=25;
  Longitude.  init(tft, "Longitude",   88, top, 136); top+=25;
  Date.       init(tft, "Date",        88, top, 136); top+=25;
  Time.       init(tft, "Time",        88, top, 136); top+=25;
  Speed.      init(tft, "Speed",       88, top, 136); top+=25;
  Course.     init(tft, "Course",      88, top, 136); top+=25;
  Altitude.   init(tft, "Altitude",    88, top, 136); top+=25;
  Sattelites. init(tft, "Sattelites",  88, top, 136); top+=25;
  Accuracy.   init(tft, "Accuracy",    88, top, 136); top+=25;
  Accuracy.alignLeft();
  Status.     init(tft, "Free heap",   88, top, 136); top+=25;
  //Status.alignLeft();

  //btnOK.initButtonUL(&tft, 24, top, 192, 30, TFT_DARKGREY, TFT_DARKGREY, TFT_BLACK, "OK", 1);
  btnOK.init(_tft, "OK", 24, top, 192); top+=25;

  //TFT_eSPI& tft, const String& label, int left, int top, int width, int height=20);
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
}

void ScreenGpsValues::drawFirstTime() {
  _tft->fillScreen(TFT_BLACK);

  for(auto label:labels) {
    label->drawFirstTime();
  }

}

void ScreenGpsValues::drawUpdate() {

  for(auto label:labels) {
    label->drawUpdate();
  }

  uint16_t t_x = 0, t_y = 0; // To store the touch coordinates
  boolean pressed = _tft->getTouch(&t_x, &t_y); // Reverse coordinates due to portrait mode

  /*
  _tft->drawFastVLine(t_x, t_y-5, 11, TFT_WHITE);
  _tft->drawFastHLine(t_x-5, t_y, 11, TFT_WHITE);
  */

  // Check if any key coordinate boxes contain the touch coordinates
  if (pressed && btnOK.contains(t_x, t_y)) {
    btnOK.updateValue(true);
    btnOK.drawUpdate();
  }
}
