#ifndef _WALL_H_
#define _WALL_H_

#include <SDL.h>

#include "aux-math.h"
#include "graphics.h"
#include "bullet.h"

typedef struct {
	ivector end1, end2;
	int ex;
} Wall;


void draw_wall(SDL_Surface *screen, Wall *w);
void draw_walls(SDL_Surface *screen, Wall *w);
int get_free_wall(Wall *w);
void do_wall(Wall *w);
void do_walls(Wall *w);
void spawn_wall(Wall *w, ivector end1, ivector end2);
Boolean wall_hit_by_bullet(Wall *w, Bullet *bl);
void wall_take_bullet(Wall *w);
Wall *wall_blocks_path(Wall *w, ivector start, ivector end);
#endif /* _WALL_H_ */
