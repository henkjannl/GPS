#include <stdio.h>
#include <math.h>

typedef struct 
{
    int numRows;
    int numCols;
    int pixels[128][64];
} screen_t;


void ClearPixels(screen_t *screen) {
	for(int x=0; x<screen->numCols; x++)
		for(int y=0; y<screen->numRows; y++)
			screen->pixels[x][y]=0;
}


void DrawPixel(screen_t *screen, int x, int y) {
	if (x>=0 && x<screen->numCols && y>=0 && y<screen->numRows) 
		screen->pixels[x][y]=1;
}


void ShowScreen(screen_t *screen) {

	for(int y=0; y<screen->numRows; y++) {
		printf("%3d ", y);
		for(int x=0; x<screen->numCols; x++)
			if (screen->pixels[x][y]==1)
				printf("X");
			else
				printf(".");
		printf("\n");
	}	
}

void drawDot(screen_t *screen, int x, int y, float r) {
	for(int i=(int) x-r; i<(int) x+r; i++) {
		float dx = i-x;
		float dy = sqrtf(r*r-dx*dx);
		for(int j=(int) y-dy; j<(int) y+dy; j++) {
			DrawPixel(screen, i, j);
		} 
	}
}

void DrawLine(screen_t *screen, int x1, int y1, int x2, int y2, float w) {

	int x0, dx, y0, dy;

	drawDot(screen, x1, y1, 0.5*w);
	drawDot(screen, x2, y2, 0.5*w);
	
	printf("printing from (%d,%d) to (%d,%d)\n", x1, y1, x2, y2);
	
	// First obtain the absolute lengths of dx and dy to check slope
	if (x2>x1) dx=x2-x1; else dx=x1-x2;
	if (y2>y1) dy=y2-y1; else dy=y1-y2;

	float l = sqrtf(dx*dx+dy*dy);
	
	if (dx>dy) {
		// We will move in horizontal direction and draw lines vertically
		float s = w * l/(2*dx);
		
		printf("- printing in X, width: %.3f projected: %.3f\n", w, s);
		
		// Revert if needed to make dx positive
		if (x2>x1) {
			x0=x1;
			dx=x2-x1;
			y0=y1;
			dy=y2-y1;
		}
		else {
			x0=x2;
			dx=x1-x2;
			y0=y2;
			dy=y1-y2;
		}

		printf("- x from %d to %d, y from %d to %d\n", x0, x0+dx, y0, y0+dy);

		for(int i=0; i<dx; i++) {
			float y=y0+dy*i/dx;
			for(int j=(int) (y-s); j<(int) (y+s); j++)
				DrawPixel(screen, x0+i, j); 
			}
	}
	else {
		// We will move in vertical  direction and draw lines horizontally
		float s = w * l/(2*dy);

		printf("- printing in Y, width: %.3f projected: %.3f\n", w, s);
		
		// Revert if needed to make dy positive
		if (y2>y1) {
			x0=x1;
			dx=x2-x1;
			y0=y1;
			dy=y2-y1;
		}
		else {
			x0=x2;
			dx=x1-x2;
			y0=y2;
			dy=y1-y2;
		}
		
		printf("- x from %d to %d, y from %d to %d\n", x0, x0+dx, y0, y0+dy);

		for(int i=0; i<dy; i++) {
			float x=x0+dx*i/dy;
			for(int j=(int) (x-s); j<(int) (x+s); j++)
				DrawPixel(screen, j, y0+i); 
			}
	}
	
	printf("\n");

}		


int main()
{
	screen_t screen;

    screen.numRows=64;
    screen.numCols=128;

	ClearPixels(&screen);

	for(int i=0; i<12; i++) {
		float fi = 2*M_PI*i/12;
		int x1 = 64 +  4 * cos(fi);
		int y1 = 32 +  4 * sin(fi);
		int x2 = 64 + 28 * cos(fi);
		int y2 = 32 + 28 * sin(fi);
		float w = 0.5+0.5*i;
		
		DrawLine(&screen, x1, y1, x2, y2, w); 
		//DrawLine(&screen, x1, y1, x2, y2, 0.9); 
		
	}


			
	ShowScreen(&screen);
	
	return 0;
}
