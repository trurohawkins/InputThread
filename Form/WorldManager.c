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
	makePlayerManager();

	return true;
}

void runWorld() {
	gameLoop = &formLoop;
	resizeScreen = &screenChanged;
	gameThread = createThread(runGame, NULL, false);
	outputThread = 0;
	if (runGraphics > 0) {
		 outputThread = createThread(outputLoop, NULL, false);
	}

	coreLoop();
}


void formLoop(float delta) {
	renderWorld();
}

void screenChanged(int x, int y) {
	worldChanged = true;
}

bool endWorld() {
	pthread_join(gameThread, NULL);
	if (runGraphics > 0) {
		pthread_join(outputThread, NULL);
	}
	
	exitCore();
	closeGame();
	exitTermInput();
	if (runGraphics > 0) {
		exitScreen();
	}
	freeWorld();
	freePlayerManager();
	return true;
}

