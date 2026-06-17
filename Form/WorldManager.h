#ifndef WORLDMANAGER
#define WORLDMANAGER
#include "../Core/threads.h"
#include "../Core/core.h"
#include "../Game/game.h"
#include "../input.h"
#include "../output.h"
#include "world.h"

bool startWorld(bool graphics);
void runWorld();
void formLoop(float delta);
bool endWorld();
#endif
