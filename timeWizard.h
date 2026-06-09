#ifndef TIMEWIZARD
#define TIMEWIZARD

#include <stdint.h>
#include <time.h>

//in nanoseconds
typedef struct {
	uint64_t perTick;

	uint64_t lastTime;
	uint64_t accumulated;

	uint64_t maxAccumulation;
	double dt;
} TimeWizard;

void initTimeWizard(TimeWizard *wiz, int ticksPerSecond);
void updateTimeWizard(TimeWizard *wiz);
void paceFunction(TimeWizard *wiz, void (*func)(float));
int consumeTicks(TimeWizard *wiz);
#endif
