#include "world.h"
int viewX = 4;
int viewY = 3;

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

void freeWorld() {
	linkedList *forms = makeList();
	for (int i = 0; i < theWorld.x * theWorld.y; i++) {
		Cell c = theWorld.map[i];
		for (int j = 0; j < FORMS_PER_CELL; j++) {
			if (c.within[j] != 0) {
				debugWrite("got a form\n");
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
			return true;
		}
	}
	return false;
}

bool removeForm(Form *f, int x, int y) {
	if (x >= 0 && y >= 0 && x < theWorld.x && y < theWorld.y) {
		Cell *c = &theWorld.map[(y*theWorld.x) + x];
		return removeFromCell(f, c);
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
	Glyph empty = {
		.br = 0,
		.bg = 0,
		.bb = 0,

		.symbol = ' '
	};
	Glyph *glyphs = calloc(viewX * viewY, sizeof(Glyph));
	int *poses = calloc(viewX * viewY, sizeof(int));
	int count = 0;
	for (int y = 0; y < viewY; y++) {
		for (int x = 0; x < viewX; x++) {
			int w = y * theWorld.x + x;
			Cell c = theWorld.map[w];
			Nub *skin = NULL;
			for (int i = 0; i < FORMS_PER_CELL; i++) {
				Form *f = c.within[i];
				if (f != 0 && f->nub != NULL) {
					skin = findNub(f, 1);
					break;
				}
			}
			y = viewY - y - 1;
			int screeni = (y+screenY/2-viewY/2) * screenX + (x+screenX/2-viewX/2);
			//int screeni = (y) * screenX + (x);
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
	renderFrame(glyphs, poses, viewX * viewY);
	free(glyphs);
	free(poses);
}

