#include "threads.h"
#include "core.h"
#include "input.h"
#include "output.h"


void *outputLoop(void *data);

int main() {
	initCore();
	
	pthread_t gameThread = createThread(gameLoop, NULL, false);
	//pthread_t outputThread = createThread(outputLoop, NULL, false);
	
	initTermInput();
	initScreen();

	coreLoop();

	printf("main exiting\n");
	pthread_join(gameThread, NULL);
	//pthread_join(outputThread, NULL);
	exitCore();
	exitTermInput();
	return 0;
}

void *outputLoop(void *data) {
	printf("thread created\n");
	
	while (atomic_load(&running)) {
		updateScreen();
	}

	printf("thread exiting\n");
	return NULL;
}
