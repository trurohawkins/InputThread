#ifndef GAME
#define GAME
typedef struct {
	int type;
	int data;
} SystemEvent;

DECLARE_SPSC(SystemEvent, SystemQueue, 256)
void *gameLoop(void *data);
void pushEvent(int type, int data);
bool popEvent(SystemEvent *se);
#endif
