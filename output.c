#include "output.h"

atomic_int windowResized = 1;

void initScreen() {
	signal(SIGWINCH, windowResizeCallback);
}

void windowResizeCallback(int sig) {
	atomic_store(&windowResized, 1);
}

void updateScreen() {
	if (atomic_exchange(&windowResized, 0)) {
		makeScreen();
	}
}

void makeScreen() {
	struct winsize w;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &w);

	printf("window size: %d, %d\n", w.ws_row, w.ws_col);
}
