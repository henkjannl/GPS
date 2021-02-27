
#ifndef _MAP_H_
#define _MAP_H_

#include <list>
#include <string>
#include "Arduino.h"
#include "TFT_eSPI.h"

typedef struct {
  int x,y;
} tPixelCoord;
 
typedef struct {
  float lon,lat;
} tMapCoord;

class Projection {
  public:
    Projection();
    void setCenterPointOnMap(double lon, double lat);
    void setCenterOnScreen(int x, int y);
    void setRotation(double rotation);
    void setScale(int pixelsOnScreen, float distanceInWorld);
    void Project(tPixelCoord *pix, tMapCoord *mapCoord);
  protected:
    void CalcScale();
    double _CenterPointOnMapLat, _CenterPointOnMapLon;
    double _scale, _rotation;
    int _CenterOnScreenX, _CenterOnScreenY;
    double _c1, _c2, _c3, _c4, _c5, _c6;
};

class Coord {
  public:
    Coord(double lon, double lat);
    void Print(HardwareSerial *serial);
  private:
    tMapCoord mapcoord;
};

// A roadsegment is part of a road
class Segment {
   public:
      void AddCoord(Coord *coord);
      void Print(HardwareSerial *serial);
      void Plot(TFT_eSPI *tft, Projection *proj) {
   private:
      std::list<Coord*> coords;
};

// A road has a name and is made up of road segments
class Road {
   public:
      Road(const char *name);
      void AddSegment(Segment *segment);
      void Print(HardwareSerial *serial);
      void Plot(TFT_eSPI *tft, Projection *proj) {
   private:
      char *_name;
      std::list<Segment*> segments;
};

// A map is currently made up of roads (will be extended with other features)
class Map {
   public:
      void AddRoad(Road *road);
      void Print(HardwareSerial *serial);
      void Plot(TFT_eSPI *tft, Projection *proj) {
   private:
      std::list<Road*> roads;
};

#endif // ends #ifndef _MAP_H_
