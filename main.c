#include "Form/WorldManager.h"
int worldX = 40;
int worldY = 20;

void moveUp(void *form, float val) {
	if (val == 1) {
		moveForm(form, 0, 1);
	}
}
void moveDown(void *form, float val) {
	if (val == 1) {
		moveForm(form, 0, -1);
	}
}
void moveLeft(void *form, float val) {
	if (val == 1) {
		moveForm(form, -1, 0);
	}
}
void moveRight(void *form, float val) {
	if (val == 1) {
		moveForm(form, 1, 0);
	}
}

int main() {
	startWorld(true);
	makeWorld(worldX, worldY);
	setFrame(worldX, worldY);

	Form *f = makeForm(0);
	Sigil *skin = createSigil(f)->data;
	skin->symbol = '@';
	skin->r = 128;
	skin->g = 128;
	skin->b = 128;
	placeForm(f, worldX/2, worldY/2);


	Player *p = makePlayer(f, 0, 0);
	addControl(p, "K0W", moveUp);
	addControl(p, "K0A", moveLeft);
	addControl(p, "K0S", moveDown);
	addControl(p, "K0D", moveRight);
	addPlayer(p);

	runWorld();
	endWorld();
	return 0;
}

