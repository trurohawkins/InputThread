#include "threads.h"
#include "core.h"
#include "input.h"
#include "output.h"

int main() {
	initCore();
	
	initTermInput();
	initScreen();

	pthread_t gameThread = createThread(gameLoop, NULL, false);
	pthread_t outputThread = createThread(outputLoop, NULL, false);
	coreLoop();

	pthread_join(gameThread, NULL);
	pthread_join(outputThread, NULL);

	exitCore();
	exitTermInput();
	exitScreen();
	return 0;
}

