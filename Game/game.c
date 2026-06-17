#include "game.h"

PollHandler gameTimer = {
	.fd = -1
};	

TimeWizard gameWiz;
int ticksPerSecond = 60;
bool gameRunning = true;

void (*gameLoop)(float) = 0;

bool initGame() {
	initPollSystem(&gamePoll, &receiveEvent);

	initTimerFd(&gameTimer, ticksPerSecond, &gameSimulation);
	addFdToPoll(&gameTimer, gamePoll.pfd);

	initTimeWizard(&gameWiz, ticksPerSecond);

	return true;
}

void *runGame(void *data) {
	while (gameRunning) {
		runPolls(gamePoll.pfd, gamePoll.polls, 16);
	}
	return NULL;
}

void exitGame() {
	if (gameTimer.fd = -1) {
		close (gameTimer.fd);
	}
	closePoll(gamePoll);
	if (frames[0].content != 0) {
		freeRenderFrames();
	}
}

void gameSimulation() {
	uint64_t expirations;
	// used for draining
	if (read(gameTimer.fd, &expirations, sizeof(expirations)) == -1) {
		perror("readding timer fd for simulations");
		return;
	}
	updateTimeWizard(&gameWiz);
	//paceFunction(&gameWiz, simulateStep);
	int steps = consumeTicks(&gameWiz);
	if (gameLoop) {
		for (int i = 0; i < steps; i++) {
			gameLoop(gameWiz.dt);
		}
	}
}

void receiveEvent() {
	// drain event fd
	uint64_t count;
	if (read(gamePoll.handler.fd, &count, sizeof(count)) == -1) {
		perror("read event fd");
		return;
	}

	// read events in queue
	SystemEvent se;
	while (popEvent(&se)) {
		if (se.type == STDIN_FILENO) {
			char c = 0;
			memcpy(&c, se.data, se.size);
			if (c == 27) {
				gameRunning = false;
				atomic_store_explicit(&running, false, memory_order_release);
				wakeEvent();
			}
		} else if (se.type == 1) {
			int data[2];
			memcpy(&data, se.data, se.size);
			if (frames[0].content != 0) {
				freeRenderFrames();
			}
			makeRenderFrames(data[0], data[1]);
		}
	}
}
