#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "window.h"

/* separate all the methods responsible for manipulating windows 
 * from the rest of the application for ease of switching to different 
 * windowing libraries.
 */

static char controls[6];

struct window *window_create(int width, int height)
{
	window_t *window = malloc(sizeof(window_t));

	if (!window) 
		return NULL;

	window->width = width;
	window->height = height;
	
	SDL_Init(SDL_INIT_VIDEO);
	
	window->screen = SDL_CreateWindow("voxel space test", 
									  SDL_WINDOWPOS_UNDEFINED, 
									  SDL_WINDOWPOS_UNDEFINED, 
									  width, 	
									  height, 
									  0
									 );

	window->renderer = SDL_CreateRenderer(window->screen, -1, 0);

	window->canvas = SDL_CreateTexture(window->renderer, 
									   SDL_PIXELFORMAT_RGB888, 
									   SDL_TEXTUREACCESS_STATIC, 
									   width, 
									   height
									  );


	if (!window->screen || !window->renderer || !window->canvas)
	{
		window_destroy(window);
		return NULL;
	}

	SDL_Delay(500); /* attempting to draw directly after initialization fails under certain window managers */

	return window;
}

void window_draw(window_t *window, unsigned int * buf)
{
	SDL_UpdateTexture(window->canvas, NULL, buf, window->width * sizeof(int));
	SDL_RenderCopy(window->renderer, window->canvas, NULL, NULL);
	SDL_RenderPresent(window->renderer);
}

void window_process_events(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		/* for now only detect quit request */
		switch (event.type)
		{
			case SDL_QUIT:
				quit_requested = 1;
		}
	}
}

int window_quit_requested(void)
{
	return quit_requested;
}

const char *window_get_keystate(void)
{
	memset(controls, 0, 6); /*zero out */

	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_UP]) controls[KEY_UP] = 1;
	if (state[SDL_SCANCODE_DOWN]) controls[KEY_DOWN] = 1;
	if (state[SDL_SCANCODE_LEFT]) controls[KEY_LEFT] = 1;
	if (state[SDL_SCANCODE_RIGHT]) controls[KEY_RIGHT] = 1;
	if (state[SDL_SCANCODE_S]) controls[KEY_S] = 1;
	if (state[SDL_SCANCODE_W]) controls[KEY_W] = 1;

	return controls;
}

void window_destroy(window_t *window)
{
	SDL_DestroyTexture(window->canvas);
	SDL_DestroyRenderer(window->renderer);
	SDL_DestroyWindow(window->screen);
	free(window);
	SDL_Quit();
}
