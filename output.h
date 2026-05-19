#ifndef OUTPUT
#define OUTPUT

#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdlib.h>

#include "core.h"

extern atomic_int windowResized;

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
} Screen;

void initScreen();
void exitScreen();

void *outputLoop(void *data);

void renderGlyph(Glyph gly, int px, int py);
void makeScreens();
Screen *allocScreen(int width, int height);
void freeScreen(Screen *s);

void windowResizeCallback(int sig);

void debugWrite(char *message);
#endif
