#include "game.h"

int gpfd = 0;
struct epoll_event gPolls[16];
PollHandler eventHandler;

SystemQueue events;

bool gameRunning = true;

void *gameLoop(void *data) {
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

