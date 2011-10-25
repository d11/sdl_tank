#ifndef _BOT_H_
#define _BOT_H_

#include <SDL.h>
#include <math.h>

#include "player.h"
#include "wall.h"
#include "bullet.h"
#include "graphics.h"
#include "aux-math.h"


typedef struct {
	float xpos, ypos;
   fvector vel;
	int x, y;
	int x_start, x_destination, x_enroute1, x_enroute2;
	int y_start, y_destination, y_enroute1, y_enroute2;
	float t;
	float friction;
	float angle;
	int ex;
	short hp;
	short max_hp;
} Bot;

int get_free_bot(Bot *bots);
void spawn_bot(Bot *b, int x, int y, float angle);
void draw_bot(SDL_Surface *screen, Bot *b);
void draw_bots(SDL_Surface *screen, Bot *bots);
void do_bot(Bot *b, Player *p, Bullet *bullets, Wall *walls, float dt);
void do_bots(Bot *bots, Player *p, Bullet *bullets, Wall *walls, float dt);
Boolean bot_hit_by_bullet(Bot *bt, Bullet *bl);
void bot_take_bullet(Bot *bt, Bullet *b);

#endif /* _BOT_H_ */
