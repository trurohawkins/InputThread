#ifndef GAME
#define GAME

#include <stdint.h>

#include "timeWizard.h"
#include "core.h"
#include "helper.h"

typedef struct {
	int type;
	int data;
} SystemEvent;

DECLARE_SPSC(SystemEvent, SystemQueue, 256);
bool initGame();
void *gameLoop(void *data);
void exitGame();
void gameSimulation();
void receiveEvent();
void pushEvent(int type, int data);
bool popEvent(SystemEvent *se);
#endif
