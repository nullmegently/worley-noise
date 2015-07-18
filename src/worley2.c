#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "worley.h"
#include "linked-list.h"
#include "context.h"

#define GRID_WIDTH 100
#define GRID_HEIGHT 100
#define NUM_NEIGHBOURS 8

static bucket_pool_t buckets;
static bucket_t *neighbours[NUM_NEIGHBOURS];

/* various dist funcs go here */
static double euclidean_distance(vec3d_t *p1, vec3d_t *p2)
{
	double dx = p2->x - p1->x;	
	double dy = p2->y - p1->y;	
	return sqrt(dx * dx + dy * dy);
}

static double manhattan_distance(vec3d_t *p1, vec3d_t *p2)
{
	return fabs(p1->x - p2->x) + fabs(p1->y - p2->y);
}
/*end dist funcs */

static void clamp(int *actual, int min, int max)
{
	if (*actual < min) *actual = min;
	if (*actual > max) *actual = max;
}

static int linked_list_sorted_insert_double(linked_list_t *list, double num)
{
	int *n = malloc(sizeof(double));
	if (!n) return 0;
	*n = num;
	return linked_list_sorted_insert(list, (void *) n);
}

static double linked_list_get_double_at(linked_list_t *list, int index)
{
	const double *n = linked_list_get_element_at(list, index);
	return *n;
}

static int double_cmp( void *a, void *b)
{
	double *x = (double *) a;
	double *y = (double *) b;
	return *x > *y;
}

static double randr(int min, int max)
{
	assert(min < max);
	return  (rand() % (max - min)) + min;
}

static int bucket_pool_valid(vec3i_t *v)
{
	int x = v->x;
	int y = v->y;

	if (x >= 0 && x < buckets.grid_width 
	 && y >= 0 && y < buckets.grid_height) 
		return 1;

	return 0;
}

static int distribute_points(bucket_t *bucket, int num)
{
	bucket->points = malloc(sizeof(vec3d_t) * num);
	if (!bucket->points) return 0;

	int i;
	for (i = 0; i < num; i++)
	{
		int sx = bucket->grid_coordinates.x;
		int sy = bucket->grid_coordinates.y;

		bucket->points[i].x = randr(sx + 1, sx + GRID_WIDTH);
		bucket->points[i].y = randr(sy + 1, sy + GRID_HEIGHT);
		bucket->points[i].z = 0;

		printf("%f %f\n", bucket->points[i].x, bucket->points[i].y);
	}
	
	return 1;
}

static int bucket_pool_init(int width, int height, int seed)
{
	/* seed for point coord generator */
	srand(seed);

	buckets.grid_width  = (int) ceil((double) width  / (double) GRID_WIDTH);
	buckets.grid_height = (int) ceil((double) height / (double) GRID_HEIGHT);
	buckets.pool = malloc(sizeof(bucket_t *) * buckets.grid_width * buckets.grid_height);

	if (!buckets.pool) return 0;

	/* clear pool */
	int n;
	for ( n = 0; n < buckets.grid_width * buckets.grid_height; n++)
		buckets.pool[n] = NULL;

	int gx = 0; 
	int gy = 0;
	int x = 0;
	int y = 0;

	for (gy = 0; gy < buckets.grid_height; gy++)
	{
		for (gx = 0; gx < buckets.grid_width; gx++)
		{
			bucket_t *bucket = malloc(sizeof(bucket_t));
			if (!bucket) return 0;

			bucket->points = NULL;
			bucket->num_points = 0;

			/* failure mallocing bucket->points */
			if (distribute_points(bucket, 30) == 0) return 0;

			bucket->start.x = x;
			bucket->start.y = y;
			bucket->start.z = 0;

			int ex = x + GRID_WIDTH;
			int ey = y + GRID_HEIGHT;
			clamp(&ex, 0, width);
			clamp(&ey, 0, height);

			bucket->end.x = ex;
			bucket->end.y = ey;
			bucket->end.z = 0;

			bucket->grid_coordinates.x = gx;
			bucket->grid_coordinates.y = gy;
			bucket->grid_coordinates.z = 0;

			int idx = gy * buckets.grid_width + gx;
			buckets.pool[idx] = bucket;

			x += GRID_WIDTH;
		}

		x = 0;
		y += GRID_HEIGHT;
	}

	return 1;
}

static void bucket_pool_free(void)
{
	if (!buckets.pool) return;

	int i = 0;
	for (i = 0; i < buckets.grid_width * buckets.grid_height; i++)
	{
		if (!buckets.pool[i]) continue;

		free(buckets.pool[i]->points);
		free(buckets.pool[i]);
	}

	free(buckets.pool);
}

static int bucket_get_neighbours(bucket_t *bucket)
{
	int lx = bucket->grid_coordinates.x - 1;		
	int rx = bucket->grid_coordinates.x + 1;		

	int ty = bucket->grid_coordinates.y - 1;		
	int by = bucket->grid_coordinates.y + 1;		

	/* store x,y positions of neighbors */
	vec3i_t nidx[NUM_NEIGHBOURS];

	/* neighbours above target bucket from left to right */
	nidx[0].x = lx;
	nidx[0].y = ty;
	nidx[1].x = bucket->grid_coordinates.x;
	nidx[1].y = ty;
	nidx[2].x = rx;
	nidx[2].y = ty;

	/* neigbours on the left and right of target bucket */
	nidx[3].x = lx;
	nidx[3].y = bucket->grid_coordinates.y;
	nidx[4].x = rx;
	nidx[4].y = bucket->grid_coordinates.y;

	/* neighbours below target bucket from left to right */
	nidx[5].x = lx;
	nidx[5].y = by;
	nidx[6].x = bucket->grid_coordinates.x;
	nidx[6].y = by;
	nidx[7].x = rx;
	nidx[7].y = by;

	int num_neighbours = 0; 

	int i;
	for (i = 0; i < NUM_NEIGHBOURS; i++)
	{
		neighbours[i] = NULL;

		if (bucket_pool_valid(&nidx[i])) 
		{
			int idx = nidx[i].y * buckets.grid_width + nidx[i].x;
			neighbours[num_neighbours++] = buckets.pool[idx];
		}
	}
	
	return num_neighbours;
}

static void worley_process_bucket(context_t *context, bucket_t *bucket, int num_neighbours, linked_list_t *dist_list, distance_func distance)
{
	int x, y, i, j;
	for (y = bucket->start.y; y < bucket->end.y; y++)
	for (x = bucket->start.x; x < bucket->end.x; x++)
	{
		for (i = 0; i < num_neighbours; i++)
		for (j = 0; j < neighbours[i]->num_points; j++)
		{
			vec3d_t screen_coords;
			screen_coords.x = x;
			screen_coords.y = y;

			double d = distance(&neighbours[i]->points[j], &screen_coords);
			linked_list_sorted_insert_double(dist_list, d);
		}

		int h = 255 - (5 * (linked_list_get_double_at(dist_list, 0)));
		clamp(&h, 0, 255);

		context_set_pixel(context, x, y, h, h, h);
	}
}


void worley_generate(context_t *context, distance_func distance, int seed)
{
	if (!bucket_pool_init(context->width, context->height, seed))
	{
		context_fill(context, 255, 0, 0);
		return;
	}

	linked_list_t *dist_list = linked_list_init(double_cmp);
	if (!dist_list)
	{
		bucket_pool_free();
		context_fill(context, 255, 0, 0);
		return;
	}

	int grid_x, grid_y;
	for (grid_y = 0; grid_y < buckets.grid_height; grid_y++)
	for (grid_x = 0; grid_x < buckets.grid_width ; grid_x++)
	{
		bucket_t *b = buckets.pool[grid_y * buckets.grid_width + grid_x];
		int nnum = bucket_get_neighbours(b);
		worley_process_bucket(context, b, nnum, dist_list, distance);
		linked_list_clear(dist_list);
	}

	linked_list_free(dist_list);
	bucket_pool_free();
}

void worley_generate_euclidean(context_t *context, int seed)
{
	worley_generate(context, euclidean_distance, seed);
}

void worley_generate_manhattan(context_t *context, int seed)
{
	worley_generate(context, manhattan_distance, seed);
}
