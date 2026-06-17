#ifndef GAME
#define GAME

#include <stdint.h>

#include "../Core/core.h"
#include "renderFrame.h"
#include "timeWizard.h"

#include "../Form/world.h"

bool initGame();
void *gameLoop(void *data);
void exitGame();
void gameSimulation();
void simulateStep(float delta);
void renderMap();
void receiveEvent();

#endif
