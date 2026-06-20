#include "inputMap.h"

InpMap *makeInp(char *inp, void (*n_func)(void*,float)) {
	InpMap *im = (InpMap*)calloc(1, sizeof(InpMap));
	strcpy(im->input, inp);
	im->func = n_func;
	return im;
}

void freeInp(void *i) {
	InpMap *im = (InpMap*)i;
	//free(im->input);
	free(im);
}

