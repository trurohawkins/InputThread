#include "output.h"

atomic_int windowResized = 0;
atomic_int newRender = 1;
Screen *bufferA = 0;
Screen *bufferB = 0;

void initScreen() {
	signal(SIGWINCH, windowResizeCallback);
	printf("\033[3J"); // clear screen
	printf("\033[?25l"); // hide cursor
	fflush(stdout);
	makeScreens();
	Glyph g;
	g.fr = 0;
	g.fg = 150;
	g.fb = 100;

	g.br = 85;
	g.bg = 50;
	g.bb = 60;

	g.symbol = '@';
	//renderGlyph(g, bufferA->width, bufferA->height);
	for (int i = 0; i < bufferA->width * bufferB->height; i++) {
		bufferA->content[i] = g;
	}
	//bufferA->content[(bufferA->width * bufferA->height) / 2] = g;
}

void render() {
	printf("\033[0m"); //reset colors
	printf("\033[2J"); // clear screen

	for (int i = 0; i < bufferA->width * bufferA->height; i++) {
		Glyph g = bufferA->content[i];
		if (g.symbol != 0) {
			int y = i / bufferA->width;
			int x = i - (y * bufferA->width);
			renderGlyph(g, x+1, y+1);
		}
	}
	fflush(stdout);
}

void renderGlyph(Glyph gly, int px, int py) {
	printf("\033[%d;%dH", py, px);
	printf("\033[38;2;%d;%d;%dm", gly.fr, gly.fg, gly.fb);
	printf("\033[48;2;%d;%d;%dm", gly.br, gly.bg, gly.bb);
	printf("%c", gly.symbol);
}

void *outputLoop(void *data) {
	while (atomic_load_explicit(&running, memory_order_acquire)) {
		bool actionTaken = false;
		if (atomic_exchange(&windowResized, 0)) {
			makeScreens();
			actionTaken = true;
		}
		if (atomic_exchange(&newRender, 0)) {
			render();
			actionTaken = true;
		}
		if (!actionTaken) {
			usleep(1000);
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

	//printf("window size: %d, %d\n", w.ws_row, w.ws_col);
	// delete old buffers
	if (bufferA != 0) {
		freeScreen(bufferA);
	}
	if (bufferB != 0) {
		freeScreen(bufferB);
	}
	// create new buffers
	bufferA = allocScreen(w.ws_col, w.ws_row);
	bufferB = allocScreen(w.ws_col, w.ws_row);
}

Screen *allocScreen(int width, int height) {
	Screen *s = calloc(1, sizeof(Screen));
	s->width = width;
	s->height = height;
	s->content = calloc(width * height, sizeof(Glyph));
	return s;
}

void freeScreen(Screen *s) {
	free(s->content);
	free(s);
}

void windowResizeCallback(int sig) {
	atomic_store(&windowResized, 1);
	atomic_store(&newRender, 1);
}

void debugWrite(char *message) {
	FILE *fptr;
	fptr = fopen("debug.log", "a");
	fprintf(fptr, message);
	fclose(fptr);
}
