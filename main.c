#include "threads.h"
#include "input.h"
#include "output.h"

#include <errno.h>

atomic_int running = 0;

void *outputThread(void *data) {
	printf("thread created\n");
	
	while (atomic_load(&running)) {
		updateScreen();
	}

	printf("thread exiting\n");
	return NULL;
}

int main() {
	running = 1;
	pthread_t thread0 = createThread(outputThread, NULL, false);
	setRaw(1);
	initScreen();
	char c;
	while(true) {
		ssize_t r = read(STDIN_FILENO, &c, 1);
		if (r == 1) {
			if (c == 27) {
				break;
			} else {
				printf("%c\n", c);
			}
		} else if (r == -1) {
			if (errno == EINTR) {
				printf("EINTR error\n");
				continue;
			} else {
				break;
			}
		}
	}
	printf("main exiting\n");
	atomic_store(&running, 0);

	pthread_join(thread0, NULL);
	setRaw(0);
	return 0;
}
