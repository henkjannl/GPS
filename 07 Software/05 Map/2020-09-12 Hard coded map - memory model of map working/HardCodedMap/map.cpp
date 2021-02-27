#include <list>
#include <string>
#include "map.h"
#include "Arduino.h"

using namespace std;

Coord::Coord(double lon, double lat) {
  _lon=lon;
  _lat=lat;
}
void Coord::Print(HardwareSerial *serial) {
  serial->printf("    %.5f,%.5f\n", _lon, _lat);
}

void Segment::AddCoord(Coord *coord) {
  coords.push_back(coord);
}

void Segment::Print(HardwareSerial *serial) {
  list<Coord*>::iterator coord;
  for (coord= coords.begin(); coord != coords.end(); coord++)
    (*coord)->Print(serial);  
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

void Map::AddRoad(Road *road) {
  roads.push_back(road);
}

void Map::Print(HardwareSerial *serial) {
  std::list<Road*>::iterator road = roads.begin();
   
  while(road != roads.end())
    {
        //serial.print(*road._name);
        //serial->print(*road._name);
        //serial->print(*road->_name);
        //serial->print(road._name);
        //serial->print(road->_name);
        //serial->print("road\n");
        //serial->printf("%s\n", (*road)->_name);
        //road.Print(&serial);
        //road*.Print(&serial);
        //road->Print(&serial);
        //*road.Print(&serial);
        //(*road).Print(&serial);
        //(*road)->Print(&serial);
        (*road)->Print(serial);
        road++;
    }
}
