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

static void buf_clear(void **buf, int size)
{
	int i; 
	for (i = 0; i < size; i++)
		buf[i] = NULL;
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

static int bucket_get_neighbours(bucket_t *bucket, bucket_t **neighbours)
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
		if (bucket_pool_valid(&nidx[i])) 
		{
			int idx = nidx[i].y * buckets.grid_width + nidx[i].x;
			neighbours[num_neighbours++] = buckets.pool[idx];
		}
	}
	
	return num_neighbours;
}

static void worley_process_bucket(bucket_t *bucket, bucket_t **neighbours, int nnum, linked_list_t *dist_list, distance_func distance)
{
	int x, y, i, j;
	for (y = bucket->start.y; y < bucket->end.y; y++)
	for (x = bucket->start.x; y < bucket->end.x; y++)
	{
		for (i = 0; i < nnum; i++)
		for (j = 0; j < neighbours[i]->num_points; j++)
		{
				
		}
	}
}


void worley_generate(context_t *context, distance_func distance, int seed)
{
	if (!bucket_pool_init(context->width, context->height, seed))
	{

	}

}

int main()
{
	/*
	int ret = bucket_pool_init(400, 300, 100);
	if (ret == 0)
	{
		bucket_pool_free();
		return 0;
	}

	int i;
	for (i = 0; i < buckets.grid_width * buckets.grid_height; i++)
	{
		bucket_t *b = buckets.pool[i];
		printf("start: %d, %d end: %d, %d\n", b->start.x, b->start.y, b->end.x, b->end.y);
	}

	int idx = 1 * buckets.grid_width + 0;
	clear_neighbours_buf();
	int n = bucket_get_neighbours(buckets.pool[idx], neighbours);
	bucket_t *a = buckets.pool[idx];
	printf("target start: %d, %d end: %d, %d\n", a->start.x, a->start.y, a->end.x, a->end.y);

	printf("%d neigbours\n", n);

	for (i = 0; i < n; i++)
	{
		bucket_t *b = neighbours[i];
		printf("start: %d, %d end: %d, %d\n", b->start.x, b->start.y, b->end.x, b->end.y);
	}

	bucket_pool_free();

	*/
	return 0;
}
