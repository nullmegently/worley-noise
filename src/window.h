#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

typedef enum 
{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_W,
	KEY_S
} keys;

static int quit_requested = 0;

typedef struct window 
{
	int width;
	int height;
	SDL_Window   *screen;
	SDL_Renderer *renderer;
	SDL_Texture  *canvas;
} window_t;

struct window *window_create(int, int);
void   window_draw(struct window *, unsigned int *);
void   window_process_events(void);
int    window_quit_requested(void);
const char *window_get_keystate(void);
void   window_destroy(struct window *);
#endif
