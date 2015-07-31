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

	worley(context, time(NULL), 7, 10, 5);
	//worley_generate_euclidean_second(context, time(NULL), 1, 2, 2);
	//worley_generate_euclidean_second_minus_first(context, time(NULL), 1, 2, 2);

	//worley_generate_manhattan_first(context, time(NULL), 1, 8, 2.5);
	//worley_generate_manhattan_second(context, time(NULL), 1, 8, 2.5);
	//worley_generate_manhattan_second_minus_first(context, time(NULL), 1, 3, 5);
	window_draw(window, context->pixels);

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
