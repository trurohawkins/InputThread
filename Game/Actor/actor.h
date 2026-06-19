#ifndef ACTOR
#define ACTOR

#include <stdbool.h>

#include "../../helper.h"

typedef struct Action {
	bool active;
	int type;
	void *data;
	int (*func)(void *data, struct Action*);
} Action;

typedef struct Actor {
	void *body;
	linkedList *actionList;
	bool active;
	bool deleteMe;
} Actor;

Action *makeAction(int type, int (*func)(void*, Action*), void *data);
Actor *makeActor(void *body);
void addAction(Actor *actor, Action *action);
void doActions(Actor *actor);
Action *findAction(Actor *actor, int type);
void deleteActor(void *a);
void freeActionList(Actor *actor);
void freeAction(Action *action);

#include "actorList.h"
#endif
