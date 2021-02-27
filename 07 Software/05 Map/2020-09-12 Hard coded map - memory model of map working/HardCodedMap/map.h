#include <list>
#include <string>
#include "Arduino.h"

using namespace std;

class Coord {
  public:
    Coord(double lon, double lat);
    void Print(HardwareSerial *serial);
  private:
    double _lon, _lat;
};

// A roadsegment is part of a road
class Segment {
   public:
      void AddCoord(Coord *coord);
      void Print(HardwareSerial *serial);
   private:
      std::list<Coord*> coords;
};

// A road has a name and is made up of road segments
class Road {
   public:
      Road(const char *name);
      void AddSegment(Segment *segment);
      void Print(HardwareSerial *serial);
   private:
      char *_name;
      std::list<Segment*> segments;
};

// A map is currently made up of roads (will be extended with other features)
class Map {
   public:
      void AddRoad(Road *road);
      void Print(HardwareSerial *serial);
   private:
      std::list<Road*> roads;
};
