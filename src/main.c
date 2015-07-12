#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include "context.h"
#include "worley.h"
#define WIDTH 512
#define HEIGHT 512 

int EXIT_STATUS = 0;

int main(int argc, char **argv)
{
	if (argc != 1)
	{
		printf("3 arguments expected - <heightmap> <diffuse>\n");
		return 1;
	}

	
	/* window + renderer */
	window_t *window = window_create(WIDTH, HEIGHT);
	context_t *context = context_create(WIDTH, HEIGHT);

	int x, y;
	for (y = 0; y < HEIGHT; y++)
	for (x = 0; x < WIDTH ; x++)
	{
		context_set_pixel(context, x, y, 255, 255, 255);
	}

	worley_generate_euclidean(context);
	//worley_generate_manhattan(context);
	window_draw(window, context->pixels);

	window_save(window, context);

	for (;;) 
	{
		window_process_events();
		if (window_quit_requested()) 
			break;
	}
	

	context_destroy(context);
	window_destroy(window);

	return 0;
}
