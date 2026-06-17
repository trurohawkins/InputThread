#include "WorldManager.h"

bool runGraphics = true;

pthread_t gameThread;
pthread_t outputThread;

bool startWorld(bool graphics) {
	initCore();

	initGame();

	initTermInput();
	if (graphics > 0) {
		initScreen();
	}
	runGraphics = graphics;
	return true;
}

void runWorld() {
	gameLoop = &formLoop;
	gameThread = createThread(runGame, NULL, false);
	outputThread = 0;
	if (runGraphics > 0) {
		 outputThread = createThread(outputLoop, NULL, false);
	}

	coreLoop();
}

bool changed = true;

void formLoop(float delta) {
	if (changed) {
		renderWorld();
		changed = false;
	}
}

bool endWorld() {
	pthread_join(gameThread, NULL);
	if (runGraphics > 0) {
		pthread_join(outputThread, NULL);
	}
	
	exitCore();
	exitGame();
	exitTermInput();
	if (runGraphics > 0) {
		exitScreen();
	}
	return true;
}

