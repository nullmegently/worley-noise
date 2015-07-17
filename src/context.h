#ifndef CONTEXT_H
#define CONTEXT_H
typedef struct context 
{
	int width; 
	int height;
	unsigned int *pixels;
} context_t;

context_t *context_create(int, int);
void context_destroy(context_t *);
void context_set_pixel(context_t *, int, int, unsigned char, unsigned char, unsigned char);
void context_fill(context_t *, unsigned int, unsigned int, unsigned int);
#endif
