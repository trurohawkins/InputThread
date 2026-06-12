#include "threads.h"
#include "core.h"
#include "timeWizard.h"
#include "input.h"
#include "output.h"
#include "Form/game.h"

int graphics = 1;

int main() {
	initCore();

	initGame();

	initTermInput();
	if (graphics > 0) {
		initScreen();
	}


	pthread_t gameThread = createThread(gameLoop, NULL, false);
	pthread_t outputThread = 0;
	if (graphics > 0) {
		 outputThread = createThread(outputLoop, NULL, false);
	}

	coreLoop();

	pthread_join(gameThread, NULL);
	if (graphics > 0) {
		pthread_join(outputThread, NULL);
	}
	
	exitCore();
	exitGame();
	exitTermInput();
	if (graphics > 0) {
		exitScreen();
	}
	return 0;
}

