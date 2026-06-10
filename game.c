#include "game.h"

PollHandler gameTimer = {
	.fd = -1
};	

SystemQueue events;
TimeWizard gameWiz;
int ticksPerSecond = 60;
bool gameRunning = true;

bool initGame() {
	initPollSystem(&gamePoll, &receiveEvent);

	initTimerFd(&gameTimer, ticksPerSecond, &gameSimulation);
	addFdToPoll(&gameTimer, gamePoll.pfd);

	initTimeWizard(&gameWiz, ticksPerSecond);
	return true;
}

void *gameLoop(void *data) {
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
}

void simulateStep(float delta) {
	printf("simulating time %f\n", delta);
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
	for (int i = 0; i < steps; i++) {
		simulateStep(gameWiz.dt);
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
			if (se.data == 27) {
				gameRunning = false;
				atomic_store_explicit(&running, false, memory_order_release);
				wakeEvent();
			}
		}
	}
}


void pushEvent(int type, int data) {
	SystemEvent se;
	se.type = type;
	se.data = data;

	AqPushResult res = SystemQueue_aqPush(&events, se);
	if (res == AQ_PUSH_SUCCESS_EMPTY) {
		uint64_t v = 1;
		if (write(gamePoll.handler.fd, &v, sizeof(v)) == -1) {
			perror("write to event");
		}
	} else if (res == AQ_PUSH_FAILED) {
		perror("event queue push fail");
	}
}

bool popEvent(SystemEvent *se) {
	bool ret = SystemQueue_aqPop(&events, se);
	return ret;
}

