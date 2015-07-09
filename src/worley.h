#ifndef WORLEY_H
#define WORLEY_H

struct context;

typedef struct vec3d
{
	double x;
	double y;
	double z;
} vec3d_t;

/* so that we can have different dist funcs */
typedef double (*distance_func)(vec3d_t *, vec3d_t *); 
void worley_generate(struct context *, distance_func);
void worley_generate_euclidean(struct context *);
void worley_generate_manhattan(struct context *);
#endif
