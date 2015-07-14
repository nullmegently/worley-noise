#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "worley.h"

#define GRID_WIDTH 100
#define GRID_HEIGHT 100

static bucket_pool_t buckets;

static void clamp(int *actual, int min, int max)
{
	if (*actual < min) *actual = min;
	if (*actual > max) *actual = max;
}

static void distribute_points(bucket_t *bucket, int seed, int num)
{

}

static int bucket_pool_init(int width, int height)
{
	buckets.grid_width  = (int) ceil((double) width  / (double) GRID_WIDTH);
	buckets.grid_height = (int) ceil((double) height / (double) GRID_HEIGHT);
	buckets.pool = malloc(sizeof(bucket_t *) * buckets.grid_width * buckets.grid_height);
	if (!buckets.pool) return 0;

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

			int idx = gy * buckets.grid_width + gx;
			buckets.pool[idx] = bucket;
			printf("%d\n", idx);

			x += GRID_WIDTH;
		}

		x = 0;
		y += GRID_HEIGHT;
	}

	return 1;
}

int main()
{
	bucket_pool_init(350,200);
	int i;
	printf("%d, %d bucket_pool\n", buckets.grid_width, buckets.grid_height);
	for (i = 0; i < buckets.grid_width * buckets.grid_height; i++)
	{
		bucket_t *b = buckets.pool[i];
		printf("start: %d, %d end: %d, %d\n", b->start.x, b->start.y, b->end.x, b->end.y);
	}
	return 0;
}
