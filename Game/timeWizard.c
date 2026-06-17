#include "timeWizard.h"

const int MAXSTEPS = 5;

static inline uint64_t nowNS() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

void initTimeWizard(TimeWizard *wiz, int ticksPerSecond) {
	wiz->perTick = 1000000000ULL / ticksPerSecond;
	wiz->lastTime = nowNS();
	wiz->accumulated = 0;
	wiz->maxAccumulation = wiz->perTick * MAXSTEPS;
	wiz->dt = 1.0 / ticksPerSecond;
}

void updateTimeWizard(TimeWizard *wiz) {
	uint64_t now = nowNS();
	uint64_t frameTime = now - wiz->lastTime;
	wiz->lastTime = now;

	uint64_t next = wiz->accumulated + frameTime;
	if (next > wiz->maxAccumulation || next < wiz-> accumulated) {
		wiz->accumulated = wiz->maxAccumulation;
	} else {
		wiz->accumulated = next;
	}
}

void paceFunction(TimeWizard *wiz, void (*func)(float)) {
	int steps = 0;

	while (wiz->accumulated >= wiz->perTick && steps < MAXSTEPS) {
		wiz->accumulated -= wiz->perTick;
		func(wiz->dt);
		steps++;
	}
}

int consumeTicks(TimeWizard *wiz) {
	int steps = 0;

	while (wiz->accumulated >= wiz->perTick && steps < MAXSTEPS) {
		wiz->accumulated -= wiz->perTick;
		steps++;
	}

	return steps;
}


