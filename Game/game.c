#include "game.h"

PollHandler gameTimer = {
	.fd = -1
};	

TimeWizard gameWiz;
int ticksPerSecond = 60;
bool gameRunning = true;
int worldX = 4;
int worldY = 3;
int viewX = 4;
int viewY = 3;
int screenX = 0;
int screenY = 0;
Form *f = 0;

bool initGame() {
	initPollSystem(&gamePoll, &receiveEvent);

	initTimerFd(&gameTimer, ticksPerSecond, &gameSimulation);
	addFdToPoll(&gameTimer, gamePoll.pfd);

	initTimeWizard(&gameWiz, ticksPerSecond);

	makeWorld(worldX, worldY);

	f = makeForm(0);
	Sigil *skin = createSigil(f)->data;
	skin->symbol = '@';
	skin->r = 128;
	skin->g = 128;
	skin->b = 128;
	/*
		 for (int x = 0; x < worldX; x++) {
		 for (int y = 0; y < worldY; y++) {
		 if (x % 2 == 0 && y % 2 == 0) {
		 placeForm(f, x, y);
		 }
		 }
		 }
		 */
	if (placeForm(f, 2, 1)) {
		debugWrite("form placed\n");
	} else {
		debugWrite("not placed\n");
	}
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
bool changed = true;

void simulateStep(float delta) {
	//printf("simulating time %f\n", delta);
	if (changed) {
		renderMap();
		changed = false;
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

void move(int xd, int yd) {
	if (moveForm(f, xd, yd)) {
		changed = true;	
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
			} else if (c == 'w') {
				move(0, 1);
			} else if (c == 'a') {
				move(-1, 0);
			} else if (c == 's') {
				move(0, -1);
			} else if (c == 'd') {
				move(1, 0);
			}
		} else if (se.type == 1) {
			int data[2];
			memcpy(&data, se.data, se.size);
			if (frames[0].content != 0) {
				freeRenderFrames();
			}
			screenX = data[0];
			screenY = data[1];
			makeRenderFrames(data[0], data[1]);
		}
	}
}


void renderMap() {
	Glyph empty = {
		.br = 0,
		.bg = 0,
		.bb = 0,

		.symbol = ' '
	};
	Glyph *glyphs = calloc(viewX * viewY, sizeof(Glyph));
	int *poses = calloc(viewX * viewY, sizeof(int));
	int count = 0;
	for (int y = 0; y < viewY; y++) {
		for (int x = 0; x < viewX; x++) {
			int w = y * theWorld.x + x;
			Cell c = theWorld.map[w];
			Nub *skin = NULL;
			for (int i = 0; i < FORMS_PER_CELL; i++) {
				Form *f = c.within[i];
				if (f != 0 && f->nub != NULL) {
					skin = findNub(f, 1);
					break;
				}
			}
			y = viewY - y - 1;
			int screeni = (y+screenY/2-viewY/2) * screenX + (x+screenX/2-viewX/2);
			//int screeni = (y) * screenX + (x);
			poses[count] = screeni;
			Glyph *g = &glyphs[count];//frame->content[fi];
			if (skin) {
				debugWrite("got skin\n");
				Sigil *sig = skin->data;
				//Figure skin = *((Figure*)f->skin);
				g->symbol = sig->symbol;
				g->fr = sig->r;
				g->fg = sig->g;
				g->fb = sig->b;
			} else {
				*g = empty;
			}
			count++;
		}
	}
	renderFrame(glyphs, poses, viewX * viewY);
	free(glyphs);
	free(poses);
}
