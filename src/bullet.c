#include "config.h"
#include "bullet.h"

void draw_bullet(SDL_Surface *screen, Bullet *b)
{

	/* THICK bullet mode 
	int i = 0;
	for (i = -2; i < 2; i++)
	{
		put_gradient_line(screen, b->ix1+i, b->iy1, b->ix2+i, b->iy2, b->color, 0x000000);
		put_gradient_line(screen, b->ix1, b->iy1+i, b->ix2, b->iy2+i, b->color, 0x000000);
	}
	*/	

	put_gradient_line(screen, b->ix1, b->iy1, b->ix2, b->iy2, b->color, 0x000000);
}

void draw_bullets(SDL_Surface *screen, Bullet *bullets)
{
	int i;
	for (i = 0; i < BULLET_MAX; i++)
	{
		if(bullets[i].ex) {draw_bullet(screen, &bullets[i]);}
	}
}

void do_bullet(Bullet *b, float dt)
{

	// update the end positions of the bullet streak
	b->x2 += b->vx*dt;
	b->y2 += b->vy*dt;

	// update the other end
	if (b->x1 >= 0 && b->x1 < DISPLAY_WIDTH && b->y1 >= 0 && b->y1 < DISPLAY_HEIGHT)
	{
		b->x1 += b->vx * 1.4 * dt; 
		b->y1 += b->vy * 1.4 * dt;
	}
	// ensure we only try to draw the bit that is onscreen
	if (b->x1 < 0) {b->y1 -= (b->vy*b->x1)/b->vx; b->x1 = 0;} 
	if (b->y1 < 0) {b->x1 -= (b->vx*b->y1)/b->vy; b->y1 = 0; } 
	if (b->x1 > DISPLAY_WIDTH - 1) {b->y1 -= (b->vy*(b->x1-DISPLAY_WIDTH + 1))/b->vx; b->x1 = DISPLAY_WIDTH - 1;} 
	if (b->y1 > DISPLAY_HEIGHT - 1) {b->x1 -= (b->vx*(b->y1-DISPLAY_HEIGHT + 1))/b->vy; b->y1 = DISPLAY_HEIGHT - 1;} 

	// kill the bullet when it has all left the screen
	if (b->x2 < 0) {b->ex = 0;}
	if (b->y2 < 0) {b->ex = 0;}
	if (b->x2 > DISPLAY_WIDTH - 1) {b->ex = 0;}
	if (b->y2 > DISPLAY_HEIGHT - 1) {b->ex = 0;}

	// update pixel coordinates
	b->ix1 = roundf(b->x1);
	b->iy1 = roundf(b->y1);
	b->ix2 = roundf(b->x2);
	b->iy2 = roundf(b->y2);
}

void do_bullets(Bullet *bullets, float dt)
{
	int i;
	for (i = 0; i < BULLET_MAX; i++)
	{
		if(bullets[i].ex) {do_bullet(&bullets[i], dt);}
	}
}

// find the next unused bullet number
Bullet *get_free_bullet(Bullet *bullets)
{
	int i = 0;
	while(bullets[i].ex == 1)
	{
		i++;
		if (i >= BULLET_MAX)
		{
			printf("Error, cannot create new bullet\n");
			i = 0;
			bullets[i].ex = 0;
		}
	}
	return(&bullets[i]);
}

void spawn_bullet(Bullet *bullets, float x, float y, float angle, float speed, int color)
{

	Bullet *b;
	b = get_free_bullet(bullets);

	b->ex = 1;
	b->vx = cos(angle) * speed;
	b->vy = sin(angle) * speed;

	b->x1 = x + b->vx;
	b->y1 = y + b->vy;

	b->x2 = x;
	b->y2 = y;

	b->color = color;
}

void spawn_explosion(Bullet *bullets, float x, float y, int density, int color)
{	
	int i;
	for (i=0; i<density; i++)
	{
		spawn_bullet(bullets, x, y, random_angle(), random_int(10) + 5, color);
	}
}

// kill a bullet
void bullet_destroy(Bullet *b)
{
	b->ex = 0;
}
