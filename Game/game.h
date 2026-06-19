#ifndef GAME
#define GAME

#include <stdint.h>
#include <ctype.h>

#include "../Core/core.h"
#include "../keys.h"


#include "renderFrame.h"
#include "timeWizard.h"
#include "player.h"
#include "Actor/actor.h"


extern void (*gameLoop)(float);
extern void (*resizeScreen)(int, int);

bool initGame();
void *runGame(void *data);
void exitGame();
void closeGame();
void gameSimulation();
void simulateStep(float delta);
void receiveEvent();

#endif
