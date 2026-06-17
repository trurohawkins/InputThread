#include "form.h"

Form *makeForm(int id) {
	Form *form = calloc(1, sizeof(Form));
	form->id = id;
	return form;
}

Nub *growNub(Form *f) {
	Nub *nub = calloc(1, sizeof(Nub));
	nub->type = 0;
	nub->data = f;
	f->nub = nub;
	return nub;
}

Nub *findNub(Form *f, int type) {
	Nub *nub = f->nub;
	while (nub) {
		if (nub->type == type) {
			break;
		} 
		nub = nub->nub;
	}
	return nub;
}

void freeForm(void *form) {
	debugWrite("freeing form");
	Form *f = form;
	if (f->nub) {
		debugWrite("freeing nub\n");
		free(f->nub->data);
		free(f->nub);
	}
	free(f);
}

#include "sigil.c"
