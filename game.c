#include "game.h"

PollHandler gameTimer = {
	.fd = -1
};	

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
	if (frames[0].content != 0) {
		freeRenderFrames();
	}
}

int pos = 0;
float interval = 0.2;
float counter = 0;
int worldSize = 0;

void simulateStep(float delta) {
	//printf("simulating time %f\n", delta);
	if (counter + delta >= interval) {
		counter -= interval;
		pos = (pos + 1) % worldSize;
		renderFrame();
	} else {
		counter += delta;
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
			worldSize = data[0] * data[1];
		}
	}
}

void renderFrame() {
	// atomic load next frame
	int next = atomic_load(&renderReadIndex);
	int newFrame = (next + 1) % NUM_FRAMES;
	//populate B
	Glyph g;
	g.fr = 0;
	g.fg = 150;
	g.fb = 100;

	g.br = 85;
	g.bg = 50;
	g.bb = 60;

	g.symbol = '@';
	Glyph empty = {
		.fr = 0,
		.fg = 0,
		.fb = 0,
		.br = 0,
		.bg = 0,
		.bb = 0,

		.symbol = ' '
	};
	for (int i = 0; i < worldSize; i++) {
		if (i != pos) {
			frames[newFrame].content[i] = empty;
		} else {
			frames[newFrame].content[pos] = g;
		}
	}
	atomic_store_explicit(&renderWriteIndex, newFrame, memory_order_release);
	//signal redraw
	setNewRender();
}
