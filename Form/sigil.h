#ifndef SIGIL
#define SIGIL
#include "form.h"

typedef struct {
	bool figure;
	char symbol;
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Sigil;

Nub *createSigil(Form *f);
void colorSigil(Form *f, int r, int g, int b);
void changeSigil(Form *f, char newSigil);
void setFigure(Form *f, bool figure);

#endif
