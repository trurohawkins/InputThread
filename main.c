#include "threads.h"
#include "core.h"
#include "timeWizard.h"
#include "input.h"
#include "output.h"
#include "game.h"

int main() {
	initCore();

	initGame();

	initTermInput();
	initScreen();


	pthread_t gameThread = createThread(gameLoop, NULL, false);
	pthread_t outputThread = createThread(outputLoop, NULL, false);

	coreLoop();

	pthread_join(gameThread, NULL);
	pthread_join(outputThread, NULL);
	
	exitCore();
	exitGame();
	exitTermInput();
	exitScreen();
	return 0;
}

