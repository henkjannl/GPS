/*
  Ellipse drawing example

  This sketch does not use any fonts.
*/

#include <M5Stack.h>
#include <math.h>

void Bresenham(int16_t x0,int16_t y0,int16_t x1,int16_t y1, uint16_t color) {
  int16_t xp=x0;
  int16_t yp=y0;
  int16_t error=0;
  int16_t dx, dy;

  if (x0>x1) {
    // Reverse the begin- and endpoint to get in the right octant
    Bresenham(x1,y1,x0,y0,color);
  }
  else {
    dx=x1-x0;

    if (y0<=y1) {
      dy=y1-y0;

      // Octant 1 and 2
      if ( dx>=dy ) {

        // Quadrant 1: x0<=x1, y0<=y1, dx>=dy
        while(xp<=x1) {
            M5.Lcd.drawPixel(xp, yp, color);
            xp+=1;
            error=error+2*dy;
            if (error>dx) {
              yp=yp+1;
              error=error-2*dx;
            }
          }
        }
      else {
        
        // Octant 2: x0<=x1, y0<=y1, dx<dy
        while(yp<=y1) {
          M5.Lcd.drawPixel(xp, yp, color);
          yp+=1;
          error=error+2*dx;
          if (error>dy) {
            xp=xp+1;
            error=error-2*dy;
          }
        }
      }
    }
    else {
      dy=y0-y1;
      if (dx>dy) {
        
        // Octant 8: x0<=x1, y0>y1, dx>dy
        while(xp<=x1) {
          M5.Lcd.drawPixel(xp, yp, color);
          xp+=1;
          error=error+2*dy;
          if (error>dx) {
            yp=yp-1;
            error=error-2*dx;
          }
        }
      }
      else {
        
        // Octant 7: x0<=x1, y0>y1, dx<=dy
        while(yp>=y1) {
          M5.Lcd.drawPixel(xp, yp, color);
          yp-=1;
          error=error+2*dx;
          if (error>dy) {
            xp=xp+1;
            error=error-2*dy;
          }
        }
      }
    }
  }
}


void setup(void) {
  M5.begin();

  // M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_BLACK);

  // 1: Test if drawing a single dot works
  Bresenham(160,120,160,120, WHITE);

  // 2: Draw multiple lines in all quadrants
  for(float angle=0; angle<2*3.1415; angle+=3.1415/17) {
    int x0=160+ 20*cos(angle);
    int y0=120+ 20*sin(angle);
    int x1=160+100*cos(angle);
    int y1=120+100*sin(angle);
    Bresenham(x0,y0,x1,y1, RED);
  }

  delay(2000);
}

void loop() {
  static int16_t x0=160,y0=120,x1,y1;

  // 3: Draw a string of lines in random directions
  x1=x0+random(11)-5;
  y1=y0+random(11)-5;

  if(x1<  0) x1=0;
  if(x1>320) x1=320;
  if(y1<  0) y1=0;
  if(y1>240) y1=240;

  Bresenham(x0,y0,x1,y1, WHITE);

  x0=x1;
  y0=y1;

  delay(20);  
}
