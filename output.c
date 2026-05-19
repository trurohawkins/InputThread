#include "output.h"

atomic_int windowResized = 0;
Screen *bufferA;
Screen *bufferB;

void initScreen() {
	signal(SIGWINCH, windowResizeCallback);
	printf("\033[3J"); // clear screen
	printf("\033[?25l"); // hide cursor
	fflush(stdout);
	makeScreens();
}

void *outputLoop(void *data) {
	while (atomic_load_explicit(&running, memory_order_acquire)) {
		if (atomic_exchange(&windowResized, 0)) {
			makeScreens();
		}
	}
	return NULL;
}

void exitScreen() {
	printf("\033[0m"); //reset colors
	printf("\033[?25h"); // show cursor
	fflush(stdout);
	freeScreen(bufferA);
	freeScreen(bufferB);
}

void makeScreens() {
	struct winsize w;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &w);

	printf("window size: %d, %d\n", w.ws_row, w.ws_col);
	// delete old buffers
	if (bufferA != 0) {
		freeScreen(bufferA);
	}
	if (bufferB != 0) {
		freeScreen(bufferB);
	}
	// create new buffers
	bufferA = allocScreen(w.ws_row, w.ws_col);
	bufferB = allocScreen(w.ws_row, w.ws_col);
}

Screen *allocScreen(int width, int height) {
	Screen *s = malloc(sizeof(Screen));
	s->width = width;
	s->height = height;
	s->content = malloc(sizeof(Glyph) * width * height);
	return s;
}

void freeScreen(Screen *s) {
	free(s->content);
	free(s);
}

void windowResizeCallback(int sig) {
	atomic_store(&windowResized, 1);
}

