#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "window.h"
#include "context.h"
#include "worley.h"
#define WIDTH 1024 
#define HEIGHT 1024 


int main(int argc, char **argv)
{
	
	/* window + renderer */
	window_t *window = window_create(WIDTH, HEIGHT);
	context_t *context = context_create(WIDTH, HEIGHT);

	worley_generate_euclidean(context, time(NULL), 2, 15);
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
