#ifndef WORLD
#define WORLD

#include "cell.h"

typedef struct {
	int x;
	int y;
	Cell *map;
} World;

extern World theWorld;

void makeWorld(int x, int y);
void freeWorld();
bool placeForm(Form *f, int x, int y);
bool removeForm(Form *f, int x, int y);
bool moveForm(Form *f, int xd, int y);
#endif
