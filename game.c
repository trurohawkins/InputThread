#include "game.h"

int gpfd = -1;
struct epoll_event gPolls[16];
PollHandler eventHandler = {
	.fd = -1
};
PollHandler timerHandler = {
	.fd = -1
};	

SystemQueue events;
TimeWizard wizboy;
int ticksPerSecond = 60;
bool gameRunning = true;

bool initGame() {
	gpfd = epoll_create1(0);
	if (gpfd == -1) {
		perror("epoll_create1 for game loop");
		return false;
	}

	eventHandler.fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (eventHandler.fd == -1) {
		perror("eventfd game eventhandler");
		return false;
	} else {
		printf("game loop event handler %i\n", eventHandler.fd);
	}
	eventHandler.func = &receiveEvent;
	addFdToPoll(&eventHandler, gpfd);

	timerHandler.fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerHandler.fd == -1) {
		perror("timerfd_create");
		return false;
	}

	uint64_t tickNS = 1000000000ULL / ticksPerSecond; 

	struct itimerspec ts = {
		.it_interval = {
			.tv_sec = 0,
			.tv_nsec = tickNS
		},
		.it_value= {
			.tv_sec = 0,
			.tv_nsec = tickNS
		}
	};
	if (timerfd_settime(timerHandler.fd, 0, &ts, NULL) == -1) {
		perror("timeerfd_settime");
		return false;
	}
	timerHandler.func = &gameSimulation;
	addFdToPoll(&timerHandler, gpfd);

	initTimeWizard(&wizboy, ticksPerSecond);
	return true;
}

void *gameLoop(void *data) {
	while (gameRunning) {
		int n = epoll_wait(gpfd, gPolls, 16, -1);
		if (n == -1) {
			if (errno == EINTR) {
				printf("EINTER error game loop\n");
				continue;
			} else {
				perror("epoll_wait game loop");
				break;
			}
		}
		for (int i = 0; i < n; i++) {
			PollHandler *handler = gPolls[i].data.ptr;
			handler->func();
		}
	}

	return NULL;
}

void exitGame() {
	if (timerHandler.fd = -1) {
		close (timerHandler.fd);
	}
	if (eventHandler.fd != -1) {
		close (eventHandler.fd);
	}
	if (gpfd != -1) {
		close(gpfd);
	}	
}

void simulateStep(float delta) {
	printf("simulating time %f\n", delta);
}

void gameSimulation() {
	uint64_t expirations;
	// used for draining, can remov expirations
	if (read(timerHandler.fd, &expirations, sizeof(expirations)) == -1) {
		perror("readding timer fd for simulations");
		return;
	}
	/*
		 for (uint64_t i = 0; i < expirations; i++) {
		 printf("game simulation\n");
		 }
		 */
	updateTimeWizard(&wizboy);
	//paceFunction(&wizboy, simulateStep);
	int steps = consumeTicks(&wizboy);
	for (int i = 0; i < steps; i++) {
		simulateStep(wizboy.dt);
	}
}

void receiveEvent() {
	// drain event fd
	uint64_t count;
	read(eventHandler.fd, &count, sizeof(count));

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
		if (write(eventHandler.fd, &v, sizeof(v)) == -1) {
			perror("write to event");
		}
	}
}

bool popEvent(SystemEvent *se) {
	bool ret = SystemQueue_aqPop(&events, se);
	return ret;
}

