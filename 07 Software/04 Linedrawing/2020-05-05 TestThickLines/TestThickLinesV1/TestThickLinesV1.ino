#include "SPI.h"
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();

void plotCircle(int x, int y, int r, uint16_t clr) {
  int r2 = r * r;
  int area = r2 << 2;
  int rr = r << 1;
  
  for (int i = 0; i < area; i++) {
    int tx = (i % rr) - r;
    int ty = (i / rr) - r;

    if (tx * tx + ty * ty <= r2)
      tft.drawPixel(x + tx, y + ty, clr);
  }
}

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
  int circleIndex;
  int x0, y0, x1, y1;
  float w;
  float angle;
  tft.init();
  tft.setRotation(1);

  tft.fillScreen(TFT_WHITE);

  tft.setTextColor(TFT_WHITE);    
  tft.setTextSize(1);
  tft.println("Circle test");

  angle=0;
  for(int x=20; x<300; x+=40) {
    w=4;
    for(int y=20; y<220; y+=40) {
      
      x0=(int) x-15*cos(angle);
      y0=(int) y-15*sin(angle);
      x1=(int) x+15*cos(angle);
      y1=(int) y+15*sin(angle);
      
      tft.fillCircle(x0, y0, (int) (w-0.5)/2, TFT_RED);
      tft.fillCircle(x , y , (int) (w-0.5)/2, TFT_RED);
      tft.fillCircle(x1, y1, (int) (w-0.5)/2, TFT_RED);
      
      plotLine(x0,y0,x1,y1,w, TFT_BLACK);
      
      w+=1.5;
    }
    angle+=TWO_PI/21;      
  }
}

void loop() {
}
