#include <stdlib.h>
#include <assert.h>
#include "context.h"
context_t *context_create(int width, int height)
{
	context_t *ctx = malloc(sizeof(context_t));	
	if (!ctx)
		return NULL;

	ctx->width  = width;
	ctx->height = height;

	ctx->pixels = malloc(sizeof(unsigned int) * width * height);
	if (!ctx->pixels)
	{
		free(ctx);
		return NULL;
	}

	return ctx;
}

void context_destroy(context_t *context)
{
	free(context->pixels);
	free(context);
}

void context_set_pixel(context_t *context, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	int i = y * context->width + x;
	assert(i < context->width * context->height);

	context->pixels[i] = 0;
	context->pixels[i] += (r << 16);
	context->pixels[i] += (g << 8);
	context->pixels[i] += (b);
}

void context_fill(context_t *context, unsigned int r, unsigned int g, unsigned int b)
{
	int x, y;
	for (y = 0; y < context->height; y++)
	for (x = 0; x < context->width ; x++)
		context_set_pixel(context, x, y, r, g, b);
}

