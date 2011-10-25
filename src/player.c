#include <math.h>

#include "config.h"
#include "player.h"

// initial setup of player
void setup_player(Player *p)
{
	p->exact_pos.x = DISPLAY_WIDTH/2;
	p->exact_pos.y = DISPLAY_HEIGHT/2;
	p->screen_pos.x = DISPLAY_WIDTH/2;
	p->screen_pos.y = DISPLAY_HEIGHT/2;
	p->vel.x = 0.0;
	p->vel.y = 0.0;
	p->angle = 0.0;
	p->y_input = 0;
	p->x_input = 0;
	p->friction = 0.99;
	p->acc = 0.1;
	p->gun_heat = 0.0;
	p->fire_heat_rate = 1.0;
	p->gun_cool_rate = 0.2;
	p->max_hp = 60;
	p->hp = p->max_hp;
	p->kills = 0;
	p->deaths = 0;
	p->total_kills = 0;
	p->total_kills_squared = 0;
}

// Convert actual coordinates into pixel coordinates
void player_get_screen_coords(Player *p)
{
	p->screen_pos.x = roundf(p->exact_pos.x);
	p->screen_pos.y = roundf(p->exact_pos.y);
}

// draw player to screen
void draw_player(SDL_Surface *screen, Player *p)
{
	polygon pol;
	pol.max = 4;
	pol.points[0] = translate_f_vector(rotate_f_vector(f_vector(15,0),p->angle), p->exact_pos);
	pol.points[1] = translate_f_vector(rotate_f_vector(f_vector(-6,12),p->angle), p->exact_pos);
	pol.points[2] = translate_f_vector(rotate_f_vector(f_vector(0,0),p->angle), p->exact_pos);
	pol.points[3] = translate_f_vector(rotate_f_vector(f_vector(-6,-12),p->angle), p->exact_pos);

	put_polygon(screen, pol, 0xFFFFFF);

	player_get_screen_coords(p);
	put_circle(screen, p->screen_pos.x, p->screen_pos.y, 5, 0xDD4444);
}

// check whether the player has died yet
void player_check_alive(Player *p, Bullet *bullets)
{
	if (p->hp <= 0) player_die(p, bullets);
}

// player death
void player_die(Player *p, Bullet *bullets)
{
	spawn_explosion(bullets, p->exact_pos.x, p->exact_pos.y, 300, 0xFF0000);
	spawn_explosion(bullets, p->exact_pos.x, p->exact_pos.y, 300, 0xFFFFFF);
	spawn_explosion(bullets, p->exact_pos.x, p->exact_pos.y, 300, 0xFFFF00);


	p->exact_pos.x = DISPLAY_WIDTH/2;
	p->exact_pos.y = DISPLAY_HEIGHT/2;
	p->hp = p->max_hp;

	// probably temp; calc some stats
	p->total_kills += p->kills;
	p->total_kills_squared += p->kills * p->kills;
	p->deaths++;
	float mean = (float)p->total_kills/(float)p->deaths;
	float stddev = sqrt(p->total_kills_squared/p->deaths - mean*mean);
	printf("\n*** Kills ***\nThis round: %i\nTotal: %i\nAverage: %f\nStandard Deviation: %f\n\n",p->kills,p->total_kills, mean, stddev);
	p->kills = 0;
}

void player_check_bounds(Player *p)
{
	// bounds check
	if (p->exact_pos.x <= PLAYER_BOUND_WIDTH) p->exact_pos.x = PLAYER_BOUND_WIDTH;
	if (p->exact_pos.y <= PLAYER_BOUND_WIDTH) p->exact_pos.y = PLAYER_BOUND_WIDTH;
	if (p->exact_pos.x >= DISPLAY_WIDTH - PLAYER_BOUND_WIDTH - 1) p->exact_pos.x = DISPLAY_WIDTH - PLAYER_BOUND_WIDTH - 1;
	if (p->exact_pos.y >= DISPLAY_HEIGHT - PLAYER_BOUND_WIDTH -1) p->exact_pos.y = DISPLAY_HEIGHT - PLAYER_BOUND_WIDTH -1;
}

void player_handle_movement(Player *p, Wall *walls, float dt)
{
	// Calculate angle based on where facing
	p->angle = atan2((p->face.y - p->exact_pos.y),(p->face.x - p->exact_pos.x));

	// apply friction
	p->vel.x *= p->friction;
	p->vel.y *= p->friction;

	// apply acceleration
	p->vel.x += p->x_input * p->acc;
	p->vel.y -= p->y_input * p->acc;

	// update position
	fvector new_pos;
	new_pos.x = p->exact_pos.x + p->vel.x*dt;
	new_pos.y = p->exact_pos.y + p->vel.y*dt;

	// check for collisions with walls
	int i;
	for (i=0; i < WALL_MAX; i++)
	{
		if (walls[i].ex)
		{
			Intersection sect;
			fvector evec1, evec2, wvec, norm, snorm;
			evec1 = f_vector(walls[i].end1.x, walls[i].end1.y);
			evec2 = f_vector(walls[i].end2.x, walls[i].end2.y);
			wvec = vector_subtract(evec1, evec2);

			norm = get_normal(wvec);

			if (dot_product_f_vectors(norm, p->vel) > 0) norm = scale_f_vector(norm, -1);

			snorm = scale_f_vector(norm, 20);

			sect = intersect_line_segments(p->exact_pos, new_pos, translate_f_vector(evec1, snorm), translate_f_vector(evec2, snorm));
			if (sect.intersects)
			{
				new_pos = translate_f_vector(sect.point, norm);
			}
		}
	}

	p->exact_pos=new_pos;

	// handle collision with screen edges
	player_check_bounds(p);
}


// update gun temperature
void player_cool_gun(Player *p, float dt)
{ 
	// cool gun down but not below zero
	if (p->gun_heat > 0) { p->gun_heat -= p->gun_cool_rate*dt; }
	if (p->gun_heat < 0) { p->gun_heat = 0; }  
}

// check whether player is willing and able to fire
Boolean player_will_fire(Player *p)
{
	return(p->fire == 1 && p->gun_heat == 0);
}

// fire the player's weapon
void player_fire(Player *p, Bullet *bullets, float phase)
{
	// Simulate 'gun heating' to control ROF
	p->gun_heat += p->fire_heat_rate;


	float spread_angle = 0.08;

	unsigned int bullet_color = 0xFFFF66;

	// Three bullets closely spaced 
	
	/*
	spawn_bullet(bullets, p->exact_pos.x, p->exact_pos.y, p->angle, 28, bullet_color);
	spawn_bullet(bullets, p->exact_pos.x, p->exact_pos.y, p->angle - spread_angle, 24, bullet_color);
	spawn_bullet(bullets, p->exact_pos.x, p->exact_pos.y, p->angle + spread_angle, 24, bullet_color);
	spawn_bullet(bullets, p->exact_pos.x, p->exact_pos.y, p->angle - spread_angle*2, 20, bullet_color);
	spawn_bullet(bullets, p->exact_pos.x, p->exact_pos.y, p->angle + spread_angle*2, 20, bullet_color);
	*/
   /*int i;*/
   /*for (i = -1; i<=1; i++)*/
   /*{*/
   /*spawn_bullet(bullets, p->exact_pos.x, p->exact_pos.y, p->angle + spread_angle*i, 10, bullet_color);*/

   /*}*/
   int num_bullets = 3;
	int i;
	for (i = 0; i<num_bullets; i++)
	{
      float angle = p->angle + cos(phase+(float)i*(M_PI*2.0/(float)num_bullets))*spread_angle;
      printf("%f\n",angle);
		spawn_bullet(bullets, p->exact_pos.x, p->exact_pos.y, angle, 10, bullet_color);

	}
}

void player_handle_weapon(Player *p, Bullet *bullets, float dt)
{
	player_cool_gun(p, dt);

   static float phase = 0;
   float wiggle_freq = 0.2;
   phase += dt*M_PI*2.0*wiggle_freq;

	if (player_will_fire(p)) {
		player_fire(p, bullets, phase);
	}
}

void do_player(Player *p, Bullet *bullets, Wall *walls, float dt)
{
	player_handle_movement(p, walls, dt);
	player_handle_weapon(p, bullets, dt);
	player_check_alive(p, bullets);
}

// test whether player was hit by a bullet
Boolean player_hit_by_bullet(Player *p, Bullet *b)
{
	float hit_radius = 10.0;
	float dist = sqrt(pow((b->x1 - p->exact_pos.x), 2) + pow((b->y1 - p->exact_pos.y), 2));

	return (dist < hit_radius);
}

// player was hit - cause damage
void player_take_bullet(Player *p, Bullet *b)
{
   p->vel.x += b->vx*0.2;
   p->vel.y += b->vy*0.2;
	p->hp -= 2;
}
