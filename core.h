#ifndef CORE
#define CORE
#include <errno.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#include "helper.h"
#include "game.h"

extern atomic_int running;

typedef struct {
	int fd;
	void (*func)(void);
} PollHandler;

bool initCore();
void exitCore();
void coreLoop();
bool addFdToPoll(PollHandler* handler);
void wakeEvent();
#endif

