#include "Form/WorldManager.h"
int worldX = 4;
int worldY = 3;

int main() {
	startWorld(true);
	makeWorld(worldX, worldY);

	Form *f = makeForm(0);
	Sigil *skin = createSigil(f)->data;
	skin->symbol = '@';
	skin->r = 128;
	skin->g = 128;
	skin->b = 128;
	/*
		 for (int x = 0; x < worldX; x++) {
		 for (int y = 0; y < worldY; y++) {
		 if (x % 2 == 0 && y % 2 == 0) {
		 placeForm(f, x, y);
		 }
		 }
		 }
		 */
	if (placeForm(f, 2, 1)) {
		debugWrite("form placed\n");
	} else {
		debugWrite("not placed\n");
	}
	runWorld();
	endWorld();
	freeWorld();
	return 0;
}

