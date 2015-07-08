#include <stdlib.h>
#include <stdio.h>
#include "window.h"
#include "context.h"
#define WIDTH  1024
#define HEIGHT 768 

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
		context_set_pixel(context, x, y, 0, 0, 255);
	}

	for (;;) 
	{
		window_draw(window, context->pixels);


		window_process_events();
		if (window_quit_requested()) 
			break;
	}
	

	exit:
		window_destroy(window);

	return 0;
}
