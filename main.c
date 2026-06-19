#include "Form/WorldManager.h"
int worldX = 40;
int worldY = 20;

typedef struct {
	int moveX;
	int moveY;

	int speed;
	int speedCounter;
} moveVar;

int move(void *form, Action *act) {
	moveVar *mv = act->data;
	Form *f = form;
	if (mv->speedCounter >= mv->speed) {
		if (mv->moveX != 0 || mv->moveY != 0) {
			moveForm(f, mv->moveX, mv->moveY);
		}
		mv->speedCounter = 0;
	} else {
		mv->speedCounter++;
	}
	return 0;
}

void moveUp(void *actor, float val) {
	Action *move = findAction(actor, 0);
	if (move) {
		moveVar *mv = move->data;
		if (val == 1) {
			mv->moveY = 1;
		} else {
			mv->moveY = 0;
		}
	}
}

void moveLeft(void *actor, float val) {
	Action *move = findAction(actor, 0);
	if (move) {
		moveVar *mv = move->data;
		if (val == 1) {
			mv->moveX = -1;
		} else {
			mv->moveX = 0;
		}
	}
}

void moveDown(void *actor, float val) {
	Action *move = findAction(actor, 0);
	if (move) {
		moveVar *mv = move->data;
		if (val == 1) {
			mv->moveY = -1;
		} else {
			mv->moveY = 0;
		}
	}
}

void moveRight(void *actor, float val) {
	Action *move = findAction(actor, 0);
	if (move) {
		moveVar *mv = move->data;
		if (val == 1) {
			mv->moveX = 1;
		} else {
			mv->moveX = 0;
		}
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
	
	moveVar mv = {
		.moveX = 0,
		.moveY = 0,
		.speed = 10,
		.speedCounter = 0,
	};
	Action *m = makeAction(0, &move, &mv);
	Actor *actor = makeFormActor(f);
	addAction(actor, m);
	addActor(actor);

	Player *p = makePlayer(actor, 0, 0);
	addControl(p, "K0W", moveUp);
	addControl(p, "K0A", moveLeft);
	addControl(p, "K0S", moveDown);
	addControl(p, "K0D", moveRight);
	addPlayer(p);

	placeForm(f, worldX/2, worldY/2);
	printForm(f);
	runWorld();
	endWorld();
	return 0;
}

