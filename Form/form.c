#include "form.h"

Form *makeForm(int id) {
	Form *form = calloc(1, sizeof(Form));
	form->id = id;
	return form;
}

void freeForm(void *form) {
	Form *f = form;
	if (f->skin != 0) {
		free(f->skin);
	}
	free(f);
}
