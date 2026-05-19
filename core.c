#include "core.h"

bool coreRunning = true;
//used by poll handlers;
void checkRunning();

SystemQueue events;
int epfd;
PollHandler exitHandler;
struct epoll_event polls[16];
atomic_int running = 0;

bool initCore() {
	epfd = epoll_create1(0);
	if (epfd == -1) {
		perror("epoll_create1");
		return false;
	}

	exitHandler.fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (exitHandler.fd == -1) {
		perror("eventfd");
		return false;
	}
	exitHandler.func = &checkRunning;
	addFdToPoll(&exitHandler);
	running = 1;
	return true;
}

void exitCore() {
	close(exitHandler.fd);
	close(epfd);
}

void coreLoop() {
	printf("core loop running\n");
	while(coreRunning) {//atomic_load_explicit(&running, memory_order_acquire)) {
		int n = epoll_wait(epfd, polls, 16, -1);
		if (n == -1) {
			if (errno == EINTR) {
				printf("EINTR error\n");
				continue;
			} else {
				perror("epoll_wait");
				break;
			}
		}
		for (int i = 0; i < n; i++) {
			PollHandler *handler = polls[i].data.ptr;
			handler->func();
		}
	}
}

bool addFdToPoll(PollHandler *handler) {
	struct epoll_event ev = {
		.events = EPOLLIN,
		.data.ptr = handler
	};
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, handler->fd, &ev) == -1) {
		perror("epoll_ctl");
		return false;
	} else {
		return true;
	}
}

void wakeEvent() {
	uint64_t v = 1;
	if (write(exitHandler.fd, &v, sizeof(v)) == -1) {
		perror("write wakeFd");
	}
}

void checkRunning() {
	if (!atomic_load_explicit(&running, memory_order_acquire)) {
		coreRunning = false;
	}
}

#include "game.c"
