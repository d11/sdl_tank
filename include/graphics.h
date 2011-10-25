#ifndef _GRAPHICS_H_

#define _GRAPHICS_H_


#include <SDL.h>

#include "aux-math.h"

void put_pixel(SDL_Surface *screen, int x, int y, int color);
void put_screen(SDL_Surface *screen, int color);
void put_circle(SDL_Surface *screen, int x, int y, float r, int color);
void put_line(SDL_Surface *screen, int x1, int y1, int x2, int y2, int color);
void put_gradient_line(SDL_Surface *screen, int x1, int y1, int x2, int y2, int color1, int color2);
void put_bezier(SDL_Surface *screen, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, float step, int color);
void put_polygon(SDL_Surface *screen, polygon p, int color);
#endif /* _GRAPHICS_H_ */
