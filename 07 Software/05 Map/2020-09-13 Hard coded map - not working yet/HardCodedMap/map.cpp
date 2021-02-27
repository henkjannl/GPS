#include <list>
#include <string>
#include "map.h"
#include "Arduino.h"
#include "TFT_eSPI.h"

#define RAD 0.0174532925199433 // multiply with this number to convert from degrees to radians


Projection::Projection() {
  _CenterPointOnMapLat=6.785;
  _CenterPointOnMapLon=52.252;
  _scale = 20/100;          // 100 meters in the real world is 20 pixels on screen
  _rotation = 0;            // north is up
  _CenterOnScreenX = 160;
  _CenterOnScreenY = 220;
  CalcScale();              // calculate the 6 coefficients to do map projection
}

void Projection::setCenterPointOnMap(double lon, double lat) {
  _CenterPointOnMapLat=lat;
  _CenterPointOnMapLon=lon;
  CalcScale();
}
  
void Projection::setCenterOnScreen(int x, int y) {
  _CenterOnScreenX = x;
  _CenterOnScreenY = y;
  CalcScale();
}
  
void Projection::setRotation(double rotation) {
  _rotation = PI*rotation/180;
  CalcScale();
}

void Projection::setScale(int pixelsOnScreen, float distanceInWorld) {
  _scale = pixelsOnScreen/distanceInWorld;
  CalcScale();
}
  
void Projection::CalcScale() {
  double P=40e6*_scale/360*sin(_rotation);
  double Q=40e6*_scale/360*cos(_rotation);
  _c1=_CenterOnScreenX-P*_CenterPointOnMapLat-Q*cos(RAD*_CenterPointOnMapLat)*_CenterPointOnMapLon;
  _c2=P;
  _c3=Q*cos(radians(_CenterPointOnMapLat));
  _c4=_CenterOnScreenY+Q*_CenterPointOnMapLat-P*cos(RAD*_CenterPointOnMapLat)*_CenterPointOnMapLon;
  _c5=-Q;
  _c6=P*cos(radians(_CenterPointOnMapLat));
}

void Projection::Project(tPixelCoord *pix, tMapCoord *mapCoord) {
  pix->x=_c1+_c2*mapCoord->lat+_c3*mapCoord->lon;
  pix->y=_c4+_c5*mapCoord->lat+_c6*mapCoord->lon;
}

Coord::Coord(double lon, double lat) {
  mapcoord.lon=lon;
  mapcoord.lat=lat;
}

void Coord::Print(HardwareSerial *serial) {
  serial->printf("    %.5f,%.5f\n", mapcoord.lon, mapcoord.lat);
}

void Coord::Project(tPixelCoord *pix, Projection *proj) {
  proj->Project(pix, &mapcoord);
}

void Segment::AddCoord(Coord *coord) {
  coords.push_back(coord);
}

void Segment::Print(HardwareSerial *serial) {
  for (list<Coord*>::iterator coord = coords.begin(); coord != coords.end(); coord++)
    (*coord)->Print(serial);  
}

void Segment::Plot(TFT_eSPI *tft, Projection *proj) {
  bool firstPointPassed=false;
  tPixelCoord p1, p2;
  
  for (list<Coord*>::iterator coord = coords.begin(); coord != coords.end(); coord++) {
    (*coord)->Project(p1, proj);
    if(firstPointPassed) tft->drawLine(p1.x, p1.y, p2.x, p2.y, TFT_WHITE);
    p2=p1;
    firstPointPassed=true;  
  }   
}

Road::Road(const char *name) {
  _name = new char[strlen(name)+1];
  strcpy(_name,name);
}

void Road::AddSegment(Segment *segment) {
  segments.push_back(segment);
}

void Road::Print(HardwareSerial *serial) {
  list<Segment*>::iterator segment;
  int i=0;

  serial->println(_name);  
  for (segment = segments.begin(); segment != segments.end(); segment++) {
    serial->printf("  Segment[%d]\n", i++);
    (*segment)->Print(serial);
  }
}

void Road::Plot(TFT_eSPI *tft, Projection *proj) {
  for (list<Segment*>::iterator segment = segments.begin(); segment != segments.end(); segment++) (*segment)->Plot(tft,proj);
}

void Map::AddRoad(Road *road) {
  roads.push_back(road);
}

void Map::Print(HardwareSerial *serial) {
  for (list<Road*>::iterator road = roads.begin(); road != roads.end(); road++) (*road)->Print(serial);
}

void Map::Plot(TFT_eSPI *tft, Projection *proj) {
  for (list<Road*>::iterator road = roads.begin(); road != roads.end(); road++) (*road)->Plot(tft,proj);
}
