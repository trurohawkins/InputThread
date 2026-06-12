#include "cell.h"

bool addToCell(Form *form, Cell *cell) {
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		if (cell->within[i] == 0) {
			cell->within[i] = form;
			return true;
		}
	}
	return false;
}

bool removeFromCell(Form *form, Cell *cell) {
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		if (cell->within[i] == form) {
			cell->within[i] = 0;
			return true;
		}
	}
	return false;
}

void printCell(Cell c) {
	printf("printing CELL %i\n", c);
	for (int i = 0; i < FORMS_PER_CELL; i++) {
		printf("[%i] %i\n", i, c.within[i]);
	}
	printf("\n");
}
