#include "wall.h"
#include "config.h"

// draw wall
void draw_wall(SDL_Surface *screen, Wall *w)
{
//	int i;
//	for (i = -2; i <= 2; i++)
//	{
		put_line(screen, w->end1.x, w->end1.y, w->end2.x, w->end2.y, 0xDDDDDD);
//	}
}

void draw_walls(SDL_Surface *screen, Wall *w)
{
	int i;
	for (i = 0; i < WALL_MAX; i++)
	{
		if(w[i].ex) {draw_wall(screen, &w[i]);}
	}
}

// find the next unused wall number
int get_free_wall(Wall *w)
{
	int i = 0;
	while(w[i].ex == 1)
	{
		i++;
		if (i >= WALL_MAX)
		{
			printf("Error, cannot create new wall\n");
			i = 0;
			w[i].ex = 0;
		}
	}
	return(i);
}

void do_wall(Wall *w)
{
	// walls are pretty inactive	
}

void do_walls(Wall *w)
{
	int i;
	for (i = 0; i < WALL_MAX; i++)
	{
		if(w[i].ex) {do_wall(&w[i]);}
	}
}

void spawn_wall(Wall *w, ivector end1, ivector end2)
{
	int n;
	n = get_free_wall(w);
	printf("spawning wall %i\n", n);
	if (w[n].ex == 0)
	{
		w[n].ex = 1;
		w[n].end1 = end1;
		w[n].end2 = end2;
	}
}

Boolean wall_hit_by_bullet(Wall *w, Bullet *bl)
{
	Intersection sect = intersect_line_segments(f_vector(w->end1.x, w->end1.y), f_vector(w->end2.x, w->end2.y), f_vector(bl->x1, bl->y1), f_vector(bl->x2, bl->y2));
	return(sect.intersects);
}

void wall_take_bullet(Wall *w)
{


}

Wall *wall_blocks_path(Wall *w, ivector start, ivector end)
{
	return(NULL);
}

