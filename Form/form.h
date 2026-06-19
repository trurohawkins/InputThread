#ifndef FORM
#define FORM
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../helper.h"
#include "../Core/core.h"

#include "../Game/Actor/actor.h"

// standard Nub types
	// 0 - no type
	// 1 - rendering nub
	// 2 - actor nub
typedef struct Nub {
	int type;
	void *data;
	// if true free data when deleting, default = false
	bool owned;
	struct Nub *nub;
} Nub;

typedef struct {
	int id;
	int pos[2];
	Nub *nub;
} Form;

Form *makeForm(int id);
Nub *growNub(Form *f);
Nub *findNub(Form *f, int type);

// attaches nub that knows actor
Actor * makeFormActor(Form *f);

void freeForm(void *form);
void freeNub(void *nub);

void printForm(Form *f);

#include "sigil.h"
#endif
