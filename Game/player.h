#ifndef PLAYER
#define PLAYER
#include "../helper.h"
#include <ctype.h>
#include "inputMap.h"

typedef struct Player {
	void *self;
	linkedList *controls;
	int num;
	void(*delFunc)(void*);
	bool active;
	bool pausePlayer;//should I process controls while game is paused?
} Player;

#include "playerManager.h"

Player *makePlayer(void *character, int num, void (*deleteFunc)(void*));
void addControl(Player *player, char *inp, void (*n_func)(void*,float));
bool cmpPlayer(void *p1, void *p2);
void freePlayer(Player *p);

#endif
