#include "actor.h"

Action *makeAction(int type, int (*func)(void*, Action*), void *data) {
	Action *a = calloc(1, sizeof(Action));
	a->type = type;
	a->active = 1;
	a->func = func;
	a->data = data;
	return a;
}

Actor *makeActor(void *body) {
	Actor *a = calloc(1, sizeof(Actor));
	a->active = 1;
	a->body = body;
	return a;
}

void addAction(Actor *actor, Action *action) {
	addToList(&(actor->actionList), action);
}

Action *findAction(Actor *actor, int type) {
	linkedList *aList = actor->actionList;
	while (aList) {
		Action *action = aList->data;
		if (action) {
			if (action->type == type) {
				return action;
			}
		}
		aList = aList->next;
	}
	return 0;
}

void doActions(Actor *actor) {
	linkedList *aList = actor->actionList;
	while (aList != 0) {
		Action *action = aList->data;
		if (action->active) {
			action->func(actor->body, action);
		}
		aList = aList->next;
	}
}

void deleteActor(void *a) {
	Actor *actor = a;
	freeActionList(actor);
	free(actor);
}

void freeActionList(Actor *actor) {
	linkedList *al = actor->actionList;
	while (al) {
		if (al->data) {
			freeAction(al->data);
		}
		al = al->next;
	}
	freeListSaveObj(&actor->actionList);
}

void freeAction(Action *action) {
	free(action);
}
#include "actorList.c"
