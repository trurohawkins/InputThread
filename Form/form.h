#ifndef FORM
#define FORM
#include <stdlib.h>
#include "../helper.h"
#include "../core.h"

typedef struct {
	int id;
	int pos[2];

	void *skin;
} Form;

Form *makeForm(int id);
void freeForm(void *form);
#endif
