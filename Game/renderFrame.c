#include "renderFrame.h"

RenderFrame frames[NUM_FRAMES];
atomic_int renderWriteIndex;
atomic_int renderReadIndex;

atomic_int newRender = 0;
atomic_int windowResized = 0;

int screenX = 0;
int screenY = 0;

void freeRenderFrames() {
	for (int i = 0; i < NUM_FRAMES; i++) {
		free(frames[i].content);
	}
}

void makeRenderFrames(int width, int height) {
	printf("making renderframes %i, %i\n", width, height);
	for (int i = 0; i < NUM_FRAMES; i++) {
		frames[i].width = width;
		frames[i].height = height;
		frames[i].content = calloc(width * height, sizeof(Glyph));
	}
	screenX = width;
	screenY = height;
}

void renderFrame(Glyph *glyphs, int *poses, int glyphCount) {
	// atomic load next frame
	int next = atomic_load(&renderReadIndex);
	int newFrame = (next + 1) % NUM_FRAMES;
	//populate B
	Glyph border = {
		.fr = 128,
		.fg = 1,
		.fb = 1,
		.br = 1,
		.bg = 1,
		.bb = 1,

		.symbol = '/'
	};
	RenderFrame *frame = frames + newFrame;
	for (int i = 0; i < frame->width * frame->height; i++) {
		frame->content[i] = border;
	}
	for (int i = 0; i < glyphCount; i++) {
		int s = poses[i];
		if (s >= 0 && s < frame->width * frame->height) {
			frame->content[s] = glyphs[i];
		}
	}
	
	atomic_store_explicit(&renderWriteIndex, newFrame, memory_order_release);
	//signal redraw
	setNewRender();
}

void setNewRender() {
	atomic_store_explicit(&newRender, 1, memory_order_release);
	uint64_t v = 1;
	if (write(outputPoll.handler.fd, &v, sizeof(v)) == -1) {
		perror("write outpoll fd");
	}
}

void windowResizeCallback(int sig) {
	atomic_store_explicit(&windowResized, 1, memory_order_release);
}
