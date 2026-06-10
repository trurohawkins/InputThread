#include "renderFrame.h"

RenderFrame frames[NUM_FRAMES];
atomic_int renderWriteIndex;
atomic_int renderReadIndex;

void freeRenderFrames() {
	debugWrite("freeing render frames\n");
	for (int i = 0; i < NUM_FRAMES; i++) {
		free(frames[i].content);
	}
}

void makeRenderFrames(int width, int height) {
	debugWrite("allocating renderframes\n");
	for (int i = 0; i < NUM_FRAMES; i++) {
		frames[i].width = width;
		frames[i].height = height;
		frames[i].content = calloc(width * height, sizeof(Glyph));
	}
}
