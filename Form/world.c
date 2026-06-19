#include "world.h"
int frameDim[2] = {0, 0};
int framePos[2] = {0, 0};
bool worldChanged = false;

World theWorld = {
	.x = 0,
	.y = 0,
	.map = 0
};

void makeWorld(int x, int y) {
	theWorld.x = x;
	theWorld.y = y;
	theWorld.map = calloc(x * y, sizeof(Cell));
}

void setFrameDimension(int x, int y) {
	frameDim[0] = x;
	frameDim[1] = y;
	worldChanged = true;
}

void setFramePosition(int x, int y) {
	if (x >= 0 && x <= theWorld.x - frameDim[0]) {
		framePos[0] = x;
	}
	if (y >= 0 && y <= theWorld.y - frameDim[1]) {
		framePos[1] = y;
	}
	worldChanged = true;
}

void moveFrame(int xd, int yd) {
	setFramePosition(framePos[0] + xd, framePos[1] + yd);
}

void freeWorld() {
	linkedList *forms = makeList();
	for (int i = 0; i < theWorld.x * theWorld.y; i++) {
		Cell c = theWorld.map[i];
		for (int j = 0; j < FORMS_PER_CELL; j++) {
			if (c.within[j] != 0) {
				addToListSingle(&forms, c.within[j]);
			}
		}
	}
	deleteList(&forms, &freeForm);
}

bool placeForm(Form *f, int x, int y) {
	if (x >= 0 && y >= 0 && x < theWorld.x && y < theWorld.y) {
		Cell *c = &theWorld.map[(y*theWorld.x) + x];
		if (addToCell(f, c)) {
			f->pos[0] = x;
			f->pos[1] = y;
			worldChanged = true;
			return true;
		}
	}
	return false;
}

bool removeForm(Form *f, int x, int y) {
	if (x >= 0 && y >= 0 && x < theWorld.x && y < theWorld.y) {
		Cell *c = &theWorld.map[(y*theWorld.x) + x];
		if (removeFromCell(f, c)) {
			worldChanged = true;
		}
	}
	return false;
}

bool moveForm(Form *f, int xd, int yd) {
	int xp = f->pos[0] + xd;
	int yp = f->pos[1] + yd;
	if (xp >= 0 && yp >=0 && xp < theWorld.x && yp < theWorld.y) {
		removeForm(f, f->pos[0], f->pos[1]);
		placeForm(f, xp, yp);
		return true;
	}
	return false;
}

void renderWorld() {
	if (!worldChanged) {
		return;
	}
	Glyph empty = {
		.br = 0,
		.bg = 0,
		.bb = 0,

		.symbol = ' '
	};
	Glyph *glyphs = calloc(frameDim[0] * frameDim[1], sizeof(Glyph));
	int *poses = calloc(frameDim[0] * frameDim[1], sizeof(int));
	int count = 0;
	for (int y = 0; y < frameDim[1]; y++) {
		for (int x = 0; x < frameDim[0]; x++) {
			int xp = x + framePos[0];
			int yp = y + framePos[1];
			int w = yp * theWorld.x + xp;
			Cell c = theWorld.map[w];
			Nub *skin = NULL;
			for (int i = 0; i < FORMS_PER_CELL; i++) {
				Form *f = c.within[i];
				if (f != 0 && f->nub != NULL) {
					skin = findNub(f, 1);
					break;
				}
			}
			y = frameDim[1] - y - 1;
			int screeni = (y+screenY/2-frameDim[1]/2) * screenX + (x+screenX/2-frameDim[0]/2);
			//int screeni = (yp) * screenX + (xp);
			poses[count] = screeni;
			Glyph *g = &glyphs[count];//frame->content[fi];
			if (skin) {
				debugWrite("got skin\n");
				Sigil *sig = skin->data;
				//Figure skin = *((Figure*)f->skin);
				g->symbol = sig->symbol;
				g->fr = sig->r;
				g->fg = sig->g;
				g->fb = sig->b;
			} else {
				*g = empty;
			}
			count++;
		}
	}
	renderFrame(glyphs, poses, frameDim[0] * frameDim[1]);
	free(glyphs);
	free(poses);
	worldChanged = false;
}

