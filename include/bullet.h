#ifndef _BULLET_H_

#define _BULLET_H_

#include <SDL.h>
#include <math.h>

#include "graphics.h"
#include "aux-math.h"

typedef struct {
	float x1, y1, x2, y2, vx, vy;
	int ix1, iy1, ix2, iy2;
	int ex;
	int color;
} Bullet ;

void draw_bullet(SDL_Surface *screen, Bullet *b);
void draw_bullets(SDL_Surface *screen, Bullet *b);
void do_bullet(Bullet *b, float dt);
void do_bullets(Bullet *bullets, float dt);
Bullet *get_free_bullet(Bullet *bullets);
void spawn_bullet(Bullet *bullets, float x, float y, float angle, float speed, int color);
void spawn_explosion(Bullet *bullets, float x, float y, int density, int color);
void bullet_destroy(Bullet *b);
#endif /* _BULLET.H_ */
