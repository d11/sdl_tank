#ifndef _AUX_MATH_H_

#define _AUX_MATH_H_


// boolean
typedef enum {FALSE=0, TRUE=1} Boolean;

// vector storage
typedef struct {int x, y;} ivector;
typedef struct {float x, y;} fvector;

ivector i_vector(int x, int y);
fvector f_vector(float x, float y);

typedef struct {
	fvector points[16];
	int max;
} polygon;


// max and min functions for integers
int max(int a, int b);
int min(int a, int b);

// find point coords on a bezier curve
int bezier_coord(int c0,int c1,int c2,int c3,float t);

// produce random numbers of various sorts
Boolean random_binary(void);
int random_int(int upper);
float random_angle(void);

// transformations
fvector scale_f_vector(fvector original, float scalar);

fvector translate_f_vector(fvector original, fvector change);
fvector rotate_f_vector(fvector original, float angle);

polygon translate_polygon(polygon p, fvector translation);
polygon rotate_polygon(polygon p, float angle);

float dot_product_f_vectors(fvector a, fvector b);

// intersection/collision testing
typedef struct {fvector point; Boolean intersects;} Intersection;
Intersection intersect_line_segments(fvector a1, fvector a2, fvector b1, fvector b2);

fvector vector_subtract(fvector v1, fvector v2);
float vector_length(fvector v);
fvector normalize(fvector v);
fvector get_normal(fvector v);

#endif /* _AUX_MATH_H_ */
