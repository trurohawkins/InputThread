#ifndef FORM
#define FORM
#include <stdint.h>
#include <stdlib.h>
#include "../helper.h"
#include "../Core/core.h"

typedef struct Nub {
	int type;
	void *data;
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
void freeForm(void *form);

#include "sigil.h"
#endif
