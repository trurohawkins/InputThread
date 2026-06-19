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
	nub->owned = false;
	// find empty nub to attach to
	Nub **tail = &f->nub;
	while (*tail) {
		tail = &(*tail)->nub;
	}
	*tail = nub;
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

Actor *makeFormActor(Form *f) {
	Nub *a = growNub(f);
	a->type = 2;
	Actor *actor = makeActor(f);
	a->data = actor;
	return actor;
}

void freeForm(void *form) {
	Form *f = form;
	Nub *n = f->nub;
	while (n) {
		Nub *next = n->nub;
		freeNub(n);
		n = next;
	}
	free(f);
}

void freeNub(void *nub) {
	Nub *n = nub;
	if (n->owned) {
		free(n->data);
	}
	free(n);
}

void printForm(Form *f) {
	printf("FORM %d - %p:\n", f->id, f);
	printf("	pos: (%i, %i)\n", f->pos[0], f->pos[1]);
	if (f->nub) {
		printf("	nubs:\n");
		Nub *n = f->nub;
		while (n) {
			printf("		nub #%d\n", n->type);
			n = n->nub;
		}
	}
}

#include "sigil.c"
