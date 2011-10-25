// miscellaneous mathematical functions and data structures
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "aux-math.h"

// vector creation functions
ivector i_vector(int x, int y)
{
	ivector i;
	i.x = x;
	i.y = y;
	return i;
}

fvector f_vector(float x, float y)
{
	fvector f;
	f.x = x;
	f.y = y;
	return f;
}

// max and min functions for integers
int max(int a, int b) { if (a>=b) { return a; } else { return b; } }
int min(int a, int b) { if (a<=b) { return a; } else { return b; } }

// max and min functions for floats
float maxf(float a, float b) { if (a>=b) { return a; } else { return b; } }
float minf(float a, float b) { if (a<=b) { return a; } else { return b; } }

// compute x or y coordinate of a point on a bezier curve
int bezier_coord(int c0,int c1,int c2,int c3,float t)
{
	int b,c;
	c = 3*(c1-c0);
	b = 3*(c2-c1) - c;
	return (c3 - c0 - c - b)*t*t*t + b*t*t + c*t + c0;
}

// random 0 or 1 with equal p of each
Boolean random_binary(void)
{
	return (rand() > RAND_MAX*0.5);
}

int random_int(int upper)
{
	return ((float)rand() / RAND_MAX * upper);
}

float random_angle(void)
{
	return ((float)rand() / RAND_MAX * M_PI * 2);
}

// transformations
fvector scale_f_vector(fvector original, float scalar)
{
	return f_vector(original.x * scalar, original.y * scalar);
}

fvector translate_f_vector(fvector original, fvector change)
{
	return f_vector(original.x + change.x, original.y + change.y);
}

fvector rotate_f_vector(fvector original, float angle)
{
	return f_vector(original.x * cos(angle) - original.y * sin(angle), original.x * sin(angle) + original.y * cos(angle));
}

polygon translate_polygon(polygon p, fvector translation)
{
	int i;
	for (i=0; i < p.max; i++)
	{
		p.points[i] = translate_f_vector(p.points[i], translation);
	}
	return p;
}

polygon rotate_polygon(polygon p, float angle)
{
	int i;
	for (i=0; i < p.max; i++)
	{
		p.points[i] = rotate_f_vector(p.points[i], angle);
	}
	return p;
}

float dot_product_f_vectors(fvector a, fvector b)
{
	return a.x * b.x + a.y * b.y;
}

// intersection/collision testing
Intersection intersect_line_segments(fvector a1, fvector a2, fvector b1, fvector b2)
{

	Intersection isn;

	// check for vertical lines - these lead to div 0
	Boolean av, bv;
	av = FALSE;
	bv = FALSE;
	float am, ac;
	if (a2.x - a1.x == 0)
	{
		av = TRUE;
	} else {
		// get equation for first line (y = mx + c)
		am = (a2.y - a1.y)/(a2.x - a1.x);
		ac = a1.x * -am + a1.y;
	}

	float bm, bc;
	if (b2.x - b1.x == 0)
	{
		bv = TRUE;
	} else {
		// equation for second line
		bm = (b2.y - b1.y)/(b2.x - b1.x);
		bc = b1.x * -bm + b1.y;
	}

	// set to true by default
	isn.intersects = TRUE;

	// get intersection point
	if (av == FALSE && bv == FALSE)
	{
		isn.point.x = (bc - ac)/(am - bm);
		isn.point.y = am * isn.point.x + ac;
	} else if (av == TRUE && bv == FALSE) {
		isn.point.x = a1.x;
		isn.point.y = bm * isn.point.x + bc;
	} else if (av == FALSE && bv == TRUE) {
		isn.point.x = b1.x;
		isn.point.y = am * isn.point.x + ac;
	} else {
		// no collision
		isn.intersects = FALSE;
	}

	

	if (isn.point.x < minf(a1.x, a2.x) || isn.point.x > maxf(a1.x, a2.x) || isn.point.y < minf(a1.y, a2.y) || isn.point.y > maxf(a1.y, a2.y) || (isn.point.x < minf(b1.x, b2.x) || isn.point.x > maxf(b1.x, b2.x) || isn.point.y < minf(b1.y, b2.y) || isn.point.y > maxf(b1.y, b2.y)))
	{
		// no collision
		isn.intersects = FALSE;
	}
	return isn;	
}

// returns vector given by v1 - v2
fvector vector_subtract(fvector v1, fvector v2)
{
	return f_vector(v1.x - v2.x, v1.y - v2.y);
}

float vector_length(fvector v)
{
	return(sqrt(v.x * v.x + v.y * v.y));
}

fvector normalize(fvector v)
{
	float length = vector_length(v);
	return(f_vector(v.x / length , v.y / length));
}

fvector get_normal(fvector v)
{
	fvector result = f_vector(-v.y, v.x);
	result = normalize(result);
	return(result);
}


