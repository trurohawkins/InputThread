#include "sigil.h"

Nub *createSigil(Form *f) {
	Nub *sigil = growNub(f);
	sigil->type = 1;
	sigil->data = calloc(1, sizeof(Sigil));
	return sigil;
}

void colorSigil(Form *f, int r, int g, int b) {
	Nub *sigil = findNub(f, 1);
	if (sigil) {
		Sigil *sig = sigil->data;
		sig->r = r;
		sig->g = g;
		sig->b = b;
	}
}

void changeSigil(Form *f, char newSigil) {
	Nub *sigil = findNub(f, 1);
	if (sigil) {
		Sigil *sig = sigil->data;
		sig->symbol = newSigil;
	}
}

void setFigure(Form *f, bool figure) {
	Nub *sigil = findNub(f, 1);
	if (sigil) {
		Sigil *sig = sigil->data;
		sig->figure = figure;
	}
}
