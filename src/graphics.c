// general graphics routines
#include "math.h"
#include "SDL.h"

#include "config.h"
#include "aux-math.h"

/*#define CHECK_ON_SCREEN*/

#ifdef CHECK_ON_SCREEN
#define CHECK_FUNC(X, Y) (X > 0 && X < DISPLAY_WIDTH && Y > 0 && Y < DISPLAY_HEIGHT)
#else
#define CHECK_FUNC(X, Y) 1
#endif

// plot a pixel of a certain colour to the screen
void put_pixel(SDL_Surface *screen, int x, int y, int color)
{
	if (CHECK_FUNC(x,y))
	{
		int ofs = y*DISPLAY_WIDTH + x;
		((unsigned int*)screen->pixels)[ofs] = color;
	}
}

// this is probably not a good idea
void put_screen(SDL_Surface *screen, int color)
{
	int ofs;
	for (ofs=0; ofs<DISPLAY_WIDTH*DISPLAY_HEIGHT; ofs++)
	{
		((unsigned int*)screen->pixels)[ofs] = color;
	}
}

// draw a circle of radius r
void put_circle(SDL_Surface *screen, int x, int y, float r, int color)
{
	//if (x>=DISPLAY_WIDTH || y>=DISPLAY_HEIGHT || x<0 || y<0) { ;}
	int i, j;
	for (i = x-r; i<= x+r; i++)
	{
		for (j = y-r; j<= y+r; j++)
		{
			if ((i-x)*(i-x)+(j-y)*(j-y) <= r*r)	
			{
				put_pixel(screen, i,j,color);
			}
		}
	}
}

// draw a line segment
void put_line(SDL_Surface *screen, int x1, int y1, int x2, int y2, int color)
{
	// find the gradient
	float m;
	m = (float)(y2-y1)/(float)(x2-x1);

	// iterate over y or x depending on gradient of line
	if (m >= 1 || m <= -1)
	{
		int y;
		if (y1<=y2){
			for(y = y1; y <= y2; y++)
			{
				put_pixel(screen, x1+(y-y1)/m, y, color);
			}
		} else {
			for(y = y1; y >= y2; y--)
			{
				put_pixel(screen, x1+(y-y1)/m, y, color);
			}
		}
	} 
	else
	{
		int x;
		if(x1<=x2)
		{
			for(x = x1; x <= x2; x++)
			{
				put_pixel(screen, x, y1+m*(x-x1), color);
			}
		} else {
			for(x = x1; x >= x2; x--)
			{
				put_pixel(screen, x, y1+m*(x-x1), color);
			}
		}
	}
}

// draw a straight line segment from (x1, y1) to (x2, y2), smoothly changing between color1 and color2
void put_gradient_line(SDL_Surface *screen, int x1, int y1, int x2, int y2, int color1, int color2)
{
	if(CHECK_FUNC(x1, y1) && CHECK_FUNC(x2, y2))
	{
		// get RGB components of colours from the given values
		int r1, g1, b1, r2, g2, b2;
		r1 = color1 >> 16 ;
		g1 = (color1 >> 8) & 0x00FF;
		b1 = color1 & 0xFF;

		r2 = color2 >> 16;
		g2 = (color2 >> 8) & 0x00FF;
		b2 = color2 & 0xFF;

		float dist, delta_r, delta_g, delta_b;

		// we round the distance up to ensure the colour value stays within accepted bounds later on
		dist = (float)((int)(sqrt((float)(pow(x2-x1,2) + pow(y2-y1,2))))+1);

		// work out how much the values must change each pixel
		delta_r = (float)(r2-r1)/dist;
		delta_g = (float)(g2-g1)/dist;
		delta_b = (float)(b2-b1)/dist;

		float m;
		m = (float)(y2-y1)/(float)(x2-x1);

		float r, g, b;
		unsigned int color;
		unsigned char ri, gi, bi;
		r = r1; g = g1; b = b1;


		// this is written out long for performance reasons, as this will be used a lot
		// (each 'quadrant' is handled separately)

		// iterate over y or x depending on gradient of line
		if (m >= 1 || m <= -1)
		{
			int y;
			if (y1<=y2)
			{
				for(y = y1; y <= y2; y++)
				{
					r += delta_r;
					g += delta_g;
					b += delta_b;
					ri = roundf(r);
					gi = roundf(g);
					bi = roundf(b);
					color = (ri << 16) | (gi << 8) | bi;
					put_pixel(screen, x1+(y-y1)/m, y, color);
				}
			}
			else
			{
				for(y = y1; y >= y2; y--)
				{
					r += delta_r;
					g += delta_g;
					b += delta_b;
					ri = roundf(r);
					gi = roundf(g);
					bi = roundf(b);
					color = (ri << 16) | (gi << 8) | bi;
					put_pixel(screen, x1+(y-y1)/m, y, color);
				}
			}
		}
		else 
		{
			int x;
			if(x1<=x2)
			{
				for(x = x1; x <= x2; x++)
				{
					r += delta_r;
					g += delta_g;
					b += delta_b;
					ri = roundf(r);
					gi = roundf(g);
					bi = roundf(b);
					color = (ri << 16) | (gi << 8) | bi;
					put_pixel(screen, x, y1+m*(x-x1), color);
				}
			}
			else
			{
				for(x = x1; x >= x2; x--)
				{
					r += delta_r;
					g += delta_g;
					b += delta_b;
					ri = roundf(r);
					gi = roundf(g);
					bi = roundf(b);
					color = (ri << 16) | (gi << 8) | bi;
					put_pixel(screen, x, y1+m*(x-x1), color);
				}
			}
		}
	}
}

void put_bezier(SDL_Surface *screen, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, float step, int color)
{
	float t;
	int old_x, old_y, new_x, new_y;
	old_x = x0;
	old_y = y0;
	for (t = step; t <= 1; t += step)
	{
		new_x = bezier_coord(x0, x1, x2, x3, t);
		new_y = bezier_coord(y0, y1, y2, y3, t);

		put_line(screen, old_x, old_y, new_x, new_y, color);

		old_x = new_x;
		old_y = new_y;
	}
}

void put_polygon(SDL_Surface *screen, polygon p, int color)
{
	int i;
	
	for (i=0; i < p.max;i++)
	{
		put_line(screen, roundf(p.points[i].x), roundf(p.points[i].y), roundf(p.points[(i+1)% p.max].x), roundf(p.points[(i+1) % p.max].y), color);
	}
}


