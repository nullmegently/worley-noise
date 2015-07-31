#ifndef WORLEY_H
#define WORLEY_H

struct context;

typedef struct vec3d
{
	double x;
	double y;
	double z;
} vec3d_t;

typedef struct vec3i
{
	int x;
	int y;
	int z;
} vec3i_t;

typedef struct bucket
{
	vec3i_t start;
	vec3i_t end;
	vec3i_t grid_coordinates;
	vec3d_t *points;
	int num_points;
} bucket_t;

typedef struct bucket_pool
{
	int grid_width; /* number of buckets in the row */
	int grid_height; /* number of buckets in the column */
	bucket_t **pool;
} bucket_pool_t;

/* so that we can have different dist funcs */
typedef double (*distance_func)(vec3d_t *, vec3d_t *); 
typedef double (*n_closest_func)(double *);
void worley_generate(struct context *, distance_func, n_closest_func, int, int, int, double);
void worley(struct context*, int, int, int, double);
#endif
