#ifndef GAME
#define GAME

#include <stdint.h>

#include "../Core/core.h"
#include "renderFrame.h"
#include "timeWizard.h"


extern void (*gameLoop)(float);

bool initGame();
void *runGame(void *data);
void exitGame();
void gameSimulation();
void simulateStep(float delta);
void receiveEvent();

#endif
