#ifndef INPUTMAP
#define INPUTMAP
#include <stdlib.h>
#include <string.h>
#include "../helper.h"
#include "../Core/core.h"

typedef struct InpMap {
	char input[4];
	float val;
	void (*func)(void*, float);
} InpMap;

typedef struct inpReceived {
	char input[4];
	float val;
} inpReceived;

InpMap *makeInp(char *inp, void (*n_func)(void*,float));
void freeInp(void *im);

#endif
