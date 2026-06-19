#ifndef WORLD
#define WORLD

#include "../Game/renderFrame.h"
#include "../Game/game.h"
#include "cell.h"

typedef struct {
	int x;
	int y;
	Cell *map;
} World;

extern World theWorld;
extern bool worldChanged;

void makeWorld(int x, int y);
void setFrameDimension(int x, int y);
void setFramePosition(int x, int y);
void moveFrame(int xd, int yd);
void freeWorld();
bool placeForm(Form *f, int x, int y);
bool removeForm(Form *f, int x, int y);
bool moveForm(Form *f, int xd, int y);
void renderWorld();
#endif
