#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "worley.h"
#include "context.h"

static vec3d_t **points;
static int points_size;

static double randr(int min, int max)
{
	return rand() % max + min;
}

static void distribute_points(int width, int height, int num)
{
	points = malloc(sizeof(vec3d_t *) * num);
	points_size = num;

	srand(time(NULL));

	int i;
	for (i = 0; i < num; i++)
	{
		vec3d_t *p = malloc(sizeof(vec3d_t));
		p->x = randr(0, width);
		p->y = randr(0, height);
		p->z = 255;
		points[i] = p;
	}
}

static double euclidean_distance(vec3d_t *p1, vec3d_t *p2)
{
	double dx = p2->x - p1->x;	
	double dy = p2->y - p1->y;	
	return sqrt(dx * dx + dy * dy);
}

static void cleanup(void)
{
	int i;
	for (i = 0; i < points_size; i++)
		free(points[i]);
	free(points);
}

void worley_generate_euclidean(context_t *context)
{
	worley_generate(context, euclidean_distance);
}

void worley_generate(context_t *context, distance_func func)
{
	distribute_points(context->width, context->height, 10);	
	int max_distance = 0;

	int x, y, i;
	for (y = 0; y < context->height; y++)
	for (x = 0; x < context->width ; x++)
	{
		double closest_dist = INT_MAX;
		vec3d_t *closest = NULL;

		for (i = 0; i < points_size; i++)
		{
			vec3d_t screen_pos;		
			screen_pos.x = x;
			screen_pos.y = y;
			
			double pdist = func(points[i], &screen_pos);		
			
			if (pdist < closest_dist)
			{
				closest_dist = pdist;
				closest = points[i];
			}

			if (pdist > max_distance)
				max_distance = pdist;
		}

		int h = (int) (closest->z / (0.05 * closest_dist));
		if (h > 255) h = 255;
		if (h < 0)   h = 0;
		context_set_pixel(context, x, y, h, h, h);
	}


	cleanup();
}
