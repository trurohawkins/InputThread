#ifndef RENDERFRAME
#define RENDERFRAME
#include <stdint.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>

#include "core.h"

typedef struct {
	char symbol;

	uint8_t fr;
	uint8_t fg;
	uint8_t fb;

	uint8_t br;
	uint8_t bg;
	uint8_t bb;
} Glyph;

typedef struct {
	int width;
	int height;
	Glyph *content;
} RenderFrame;

#define NUM_FRAMES 3
extern RenderFrame frames[NUM_FRAMES];
extern atomic_int renderWriteIndex;
extern atomic_int renderReadIndex;

void freeRenderFrames();
void makeRenderFrames(int width, int height);
#endif
