#include "player.h"
#include "playerManager.c"

Player *makePlayer(void *character, int num, void (*deleteFunc)(void*)) {
	Player *p = (Player *)calloc(1, sizeof(Player));
	p->num = num;
	p->self = character;
	p->controls = makeList();
	p->delFunc = deleteFunc;
	p->active = true;

	//Player *a = addPlayer(p);
	return p;
}

void addControl(Player *player, char *inp, void (*n_func)(void*,float)) {
	InpMap *im = makeInp(inp, n_func);
	addToList(&(player->controls),im);
}

void freePlayer(Player *p) {
	deleteList(&p->controls, freeInp);
	if (p->delFunc != NULL) {
		p->delFunc(p->self);
	}
	free(p);
}

bool cmpPlayer(void *p1, void *p2) {
	return ((Player*)p1)->num == ((Player*)p2)->num;
}

