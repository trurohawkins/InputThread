#include "core.h"

//used by poll handlers;
void checkRunning();

atomic_int running = 0;

PollSystem corePoll = {
	.pfd = -1,
	.handler = {
		.fd = -1,
	},
};

PollSystem outputPoll = {
	.pfd = -1,
	.handler = {
		.fd = -1,
	},
};

PollSystem gamePoll = {
	.pfd = -1,
	.handler = {
		.fd = -1,
	},
};

bool initCore() {
	initPollSystem(&corePoll, &checkRunning);
	running = 1;
	return true;
}

void exitCore() {
	if (corePoll.handler.fd != -1) {
		close(corePoll.handler.fd);
	}
	if (corePoll.pfd != -1) {
		close(corePoll.pfd);
	}
}

void coreLoop() {
	while (atomic_load_explicit(&running, memory_order_acquire)) {
		runPolls(corePoll.pfd, corePoll.polls, 16);
	}
}

void wakeEvent() {
	uint64_t v = 1;
	if (write(corePoll.handler.fd, &v, sizeof(v)) == -1) {
		perror("write wakeFd");
	}
	if (write(outputPoll.handler.fd, &v, sizeof(v)) == -1) {
		perror("write outpoll fd");
	}
}

void checkRunning() {
	//drain corePoll.handler
	uint64_t v;
	read(corePoll.handler.fd, &v, sizeof(v));

	if (!atomic_load_explicit(&running, memory_order_acquire)) {
		uint64_t v;
		read(corePoll.handler.fd, &v, sizeof(v));
	}
}

bool addFdToCore(PollHandler *handler) {
	return addFdToPoll(handler, corePoll.pfd);
}


bool initTimerFd(PollHandler *handler, int ticksPerSecond, void (*func)(void)) {
	handler->fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (handler->fd == -1) {
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
	if (timerfd_settime(handler->fd, 0, &ts, NULL) == -1) {
		perror("timeerfd_settime");
		return false;
	}
	handler->func = func;
	return true;
}

