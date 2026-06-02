#ifndef GAME
#define GAME

#include <stdint.h>

#include "core.h"
#include "helper.h"

typedef struct {
	int type;
	int data;
} SystemEvent;

DECLARE_SPSC(SystemEvent, SystemQueue, 256)
void *gameLoop(void *data);
void receiveEvent();
void pushEvent(int type, int data);
bool popEvent(SystemEvent *se);
#endif
