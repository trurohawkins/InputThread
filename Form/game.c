#include "game.h"

PollHandler gameTimer = {
	.fd = -1
};	

TimeWizard gameWiz;
int ticksPerSecond = 60;
bool gameRunning = true;
int worldX = 10;
int worldY = 10;
int frameX = 10;
int frameY = 10;

Form *f = 0;

bool initGame() {
	initPollSystem(&gamePoll, &receiveEvent);

	initTimerFd(&gameTimer, ticksPerSecond, &gameSimulation);
	addFdToPoll(&gameTimer, gamePoll.pfd);

	initTimeWizard(&gameWiz, ticksPerSecond);

	makeWorld(worldX, worldY);

	f = makeForm(0);
	termSkin *skin = calloc(1, sizeof(termSkin));
	skin->symbol = '@';
	skin->r = 128;
	skin->g = 128;
	skin->b = 128;
	f->skin = skin;
	placeForm(f, 0, 0);
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
	freeWorld();
}

int pos = 0;
float interval = 0.2;
float counter = 0;
int worldSize = 100;

void simulateStep(float delta) {
	//printf("simulating time %f\n", delta);
	if (counter + delta >= interval) {
		counter -= interval;
		removeForm(f, f->pos[0], f->pos[1]);
		f->pos[1] = (f->pos[1] + 1) % worldY;
		placeForm(f, f->pos[0], f->pos[1]);
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
			frameX = data[0];
			frameY = data[1];
			worldSize = data[0] * data[1];
		}
	}
}

void renderFrame() {
	// atomic load next frame
	int next = atomic_load(&renderReadIndex);
	int newFrame = (next + 1) % NUM_FRAMES;
	//populate B
	Glyph empty = {
		.fr = 128,
		.fg = 1,
		.fb = 1,
		.br = 1,
		.bg = 1,
		.bb = 1,

		.symbol = '!'
	};
	for (int i = 0; i < frameX * frameY; i++) {
		frames[newFrame].content[i] = empty;
	}
	for (int i = 0; i < worldX * worldY; i++) {
		int fy = i / theWorld.x;//frames[newFrame].width;
		int fx = (i - (fy * theWorld.x));
		fy = theWorld.y - fy - 1;
		Cell c = theWorld.map[i];
		for (int j = 0; j < FORMS_PER_CELL; j++) {
			Form *f = c.within[j];
			if (f != 0 && f->skin != 0) {
				termSkin skin = *((termSkin*)f->skin);
				Glyph *g = &(frames[newFrame].content[(fy*frames[newFrame].width)+fx]);
				g->symbol = skin.symbol;
				g->fr = skin.r;
				g->fg = skin.g;
				g->fb = skin.b;
				break;
			}
		}
	}


		atomic_store_explicit(&renderWriteIndex, newFrame, memory_order_release);
		//signal redraw
		setNewRender();
	}
