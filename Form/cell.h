#ifndef CELL
#define CELL

#include "form.h"

#define FORMS_PER_CELL 5
typedef struct {
	Form *within[FORMS_PER_CELL];
} Cell;

bool addToCell(Form *form, Cell *cell);
bool removeFromCell(Form *form, Cell *cell);
void printCell(Cell c);

#endif
