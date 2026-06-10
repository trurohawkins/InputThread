#ifndef GAME
#define GAME

#include <stdint.h>

#include "timeWizard.h"
#include "core.h"

#include "renderFrame.h"
#include "output.h"

bool initGame();
void *gameLoop(void *data);
void exitGame();
void gameSimulation();
void simulateStep(float delta);
void renderFrame();
void receiveEvent();
#endif
