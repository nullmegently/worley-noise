#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "worley.h"

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

static int bucket_pool_valid(int actual)
{
	if (actual >= 0 && actual < buckets.grid_width * buckets.grid_height) 
		return 1;
	return 0;
}

static void clear_neighbours_buf(void)
{
	int i; 
	for (i = 0; i < NUM_NEIGHBOURS; i++)
		neighbours[i] = NULL;
}

static void distribute_points(bucket_t *bucket, int num)
{

}

static int bucket_pool_init(int width, int height, int seed)
{
	/* seed for point coord generator */
	srand(seed);

	buckets.grid_width  = (int) ceil((double) width  / (double) GRID_WIDTH);
	buckets.grid_height = (int) ceil((double) height / (double) GRID_HEIGHT);
	buckets.pool = malloc(sizeof(bucket_t *) * buckets.grid_width * buckets.grid_height);

	if (!buckets.pool) return 0;

	/* set array contents to NULL */
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
			distribute_points(bucket, 30);

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
	int j = 0;
	for (i = 0; i < buckets.grid_width * buckets.grid_height; i++)
	{
		if (!buckets.pool[i]) continue;

		for (j = 0; j < buckets.pool[i]->num_points; j++)
		{
			if (!buckets.pool[i]->points[j]) continue;
			free(buckets.pool[i]->points[j]);
		}

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

	/* at most 8 neighbours expected */
	int nidx[NUM_NEIGHBOURS];

	/* neighbours above target bucket from left to right */
	nidx[0] = ty * buckets.grid_width + lx; 
	nidx[1] = ty * buckets.grid_width + bucket->grid_coordinates.x;
	nidx[2] = ty * buckets.grid_width + rx; 

	/* neigbours on the left and right of target bucket */
	nidx[3] = bucket->grid_coordinates.y * buckets.grid_width + lx;
	nidx[4] = bucket->grid_coordinates.y * buckets.grid_width + rx;

	/* neighbours below target bucket from left to right */
	nidx[5] = by * buckets.grid_width + lx; 
	nidx[6] = by * buckets.grid_width + bucket->grid_coordinates.x;
	nidx[7] = by * buckets.grid_width + rx; 
	
	int num_neighbours = 0; 

	int i;
	for (i = 0; i < NUM_NEIGHBOURS; i++)
	{
		if (bucket_pool_valid(nidx[i])) 
			neighbours[num_neighbours++] = buckets.pool[nidx[i]];
	}
	
	return num_neighbours;
}

int main()
{
	int ret = bucket_pool_init(350,300, 100);
	if (ret == 0)
	{
		bucket_pool_free();
		return 0;
	}

	int i;
	/*
	for (i = 0; i < buckets.grid_width * buckets.grid_height; i++)
	{
		bucket_t *b = buckets.pool[i];
		printf("start: %d, %d end: %d, %d\n", b->start.x, b->start.y, b->end.x, b->end.y);
	}
	*/

	int idx = 1 * buckets.grid_width + 1;
	clear_neighbours_buf();
	int n = bucket_get_neighbours(buckets.pool[idx], neighbours);

	printf("%d neigbours\n", n);

	for (i = 0; i < n; i++)
	{
		bucket_t *b = neighbours[i];
		printf("start: %d, %d end: %d, %d\n", b->start.x, b->start.y, b->end.x, b->end.y);
	}

	bucket_pool_free();
	return 0;
}
