#include "SPI.h"
#include "TFT_eSPI.h"
#include "map.h"

TFT_eSPI tft = TFT_eSPI();

void plotLineWidth(int x0, int y0, int x1, int y1, float wd, uint16_t clr) { 
   int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1; 
   int dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1; 
   int err = dx-dy, e2, x2, y2;                          /* error value e_xy */
   float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);
   
   for (wd = (wd+1)/2; ; ) {                                   /* pixel loop */
      clr=max((uint8_t)0,(uint8_t)(255*(abs(err-dx+dy)/ed-wd+1)));
      tft.drawPixel(x0,y0,tft.color565(clr,clr,clr));
      e2 = err; x2 = x0;
      if (2*e2 >= -dx) {                                           /* x step */
         for (e2 += dy, y2 = y0; e2 < ed*wd && (y1 != y2 || dx > dy); e2 += dx) {
            clr=max((uint8_t)0,(uint8_t)(255*((abs(e2)/ed-wd+1))));
            tft.drawPixel(x0,y2 += sy,clr);
         }
         if (x0 == x1) break;
         e2 = err; err -= dy; x0 += sx; 
      } 
      if (2*e2 <= dy) {                                            /* y step */
         for (e2 = dx-e2; e2 < ed*wd && (x1 != x2 || dx < dy); e2 += dy) {
            clr=max((uint8_t)0,(uint8_t)(255*((abs(e2)/ed-wd+1))));
            tft.drawPixel(x2 += sx, y0,clr);
         }
         if (y0 == y1) break;
         err += dx; y0 += sy; 
      }
   }
}


void plotLine(int x1, int y1, int x2, int y2, int w, uint16_t color) { 
  float dx = x2-x1;
  float dy = y2-y1;

  // Determine if we should draw horizontally or vertically
  if (abs(dx)>abs(dy)) {
    // We draw horizontally, sublines are vertical
  
    if (dx<0) {
      // Swap points so we go from left to right
      dx=-dx; x1=x2; x2=x1+dx; 
      dy=-dy; y1=y2; y2=y1+dy; 
    }

    for(int x=x1; x<x2; x++) {
      int y=(int) y1+dy*(x-x1)/dx;
      for(int s=-w>>1; s<w>>1; s++)
        tft.drawPixel(x,y+s, color);
    }
    
  } 
  else {
    // We draw vertically, sublines are horizontal
    if (dy<0) {
      // Swap points so we go from bottom to top
      dx=-dx; x1=x2; x2=x1+dx; 
      dy=-dy; y1=y2; y2=y1+dy; 
    }

    for(int y=y1; y<y2; y++) {
      int x=(int) x1+(dx*(y-y1)/dy);
      for(int s=-w>>1; s<w>>1; s++)
        tft.drawPixel(x+s,y, color);
    }
    
  }
}


typedef struct {
  int radius;  
  int width;
  uint16_t color;
} type_circles;


void setup() {
  Map map;
  Road* road;
  Segment* segment;
  Coord* coord;

  Serial.begin(115200);

  road=new Road("Breemarsweg");
  segment = new Segment();
  segment->AddCoord(coord = new Coord(6.79106, 52.25161));
  segment->AddCoord(coord = new Coord(6.79012, 52.25172));
  segment->AddCoord(coord = new Coord(6.78786, 52.25202));
  segment->AddCoord(coord = new Coord(6.78493, 52.25247));
  segment->AddCoord(coord = new Coord(6.78487, 52.25248));
  segment->AddCoord(coord = new Coord(6.78376, 52.25265));
  segment->AddCoord(coord = new Coord(6.78235, 52.25286));
  segment->AddCoord(coord = new Coord(6.77874, 52.25333));
  segment->AddCoord(coord = new Coord(6.77706, 52.25361));
  segment->AddCoord(coord = new Coord(6.77603, 52.25376));
  segment->AddCoord(coord = new Coord(6.77511, 52.25389));
  segment->AddCoord(coord = new Coord(6.77411, 52.25402));
  segment->AddCoord(coord = new Coord(6.77230, 52.25431));
  road->AddSegment(segment);
  map.AddRoad(road);
  

  road = new Road("Zonstraat");
  segment=new Segment();
  segment->AddCoord(coord = new Coord(6.78487, 52.25248));
  segment->AddCoord(coord = new Coord(6.78513, 52.25321));
  segment->AddCoord(coord = new Coord(6.78532, 52.25339));
  segment->AddCoord(coord = new Coord(6.78558, 52.25354));
  segment->AddCoord(coord = new Coord(6.78634, 52.25385));
  segment->AddCoord(coord = new Coord(6.78767, 52.25398));
  road->AddSegment(segment);

  segment=new Segment();
  segment->AddCoord(coord = new Coord(6.78532, 52.25339));
  segment->AddCoord(coord = new Coord(6.78590, 52.25336));
  road->AddSegment(segment);
  map.AddRoad(road);

  road = new Road("Maanstraat");
  segment=new Segment();
  segment->AddCoord(coord = new Coord(6.78590, 52.25336));
  segment->AddCoord(coord = new Coord(6.78623, 52.25335));
  segment->AddCoord(coord = new Coord(6.78773, 52.25340));
  road->AddSegment(segment);
  map.AddRoad(road);

  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);
  map.Print(&Serial);

  Projection proj();
  map.Plot(&tft, &proj);
}

void loop() {
}
