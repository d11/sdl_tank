#ifndef _PLAYER_H_

#define _PLAYER_H_


#include <SDL.h>

#include "aux-math.h"
#include "wall.h"
#include "graphics.h"
#include "bullet.h"

typedef struct {
	fvector exact_pos; // actual, exact player coordinates 
	fvector vel; // player velocities
	ivector screen_pos; // screen (pixel) coordinates
	ivector face; 					// coordinates player wants to face towards
	float angle;
	float acc;
	float friction;				// friction coefficient applied to player movement
	float gun_heat, fire_heat_rate, gun_cool_rate;	// variables controlling weapon ROF
	signed char x_input, y_input, fire; // input from keyboard
	short hp; // hit points (health)
	short max_hp; // maximum hit points

	// stats (should they be stored elsewhere?)
	int kills;
	int total_kills;
	int total_kills_squared;
	int deaths;
} Player;

void setup_player(Player *p);
void player_get_screen_coords(Player *p);
void player_check_alive(Player *p, Bullet *bullets);
void player_die(Player *p, Bullet *bullets);
void player_check_bounds(Player *p);
void player_handle_movement(Player *p, Wall *walls, float dt);
void player_cool_gun(Player *p, float dt);
Boolean player_will_fire(Player *p);
void player_fire(Player *p, Bullet *bullets, float phase);
void player_handle_weapon(Player *p, Bullet *bullets, float dt);
void draw_player(SDL_Surface *screen, Player *p);
void do_player(Player *p, Bullet *bullets, Wall *walls, float dt);
Boolean player_hit_by_bullet(Player *p, Bullet *b);
void player_take_bullet(Player *p, Bullet *b);
#endif /* _PLAYER_H_ */
