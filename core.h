#ifndef CORE
#define CORE
#include <errno.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>

#include "helper.h"
#include "poll.h"
#include "game.h"

extern atomic_int running;

extern PollSystem outputPoll;
extern PollSystem gamePoll;

bool initCore();
void exitCore();
void coreLoop();
bool addFdToCore(PollHandler* handler);
bool initTimerFd(PollHandler *handler, int ticksPerSecond, void (*func)(void));
void wakeEvent();
#endif

