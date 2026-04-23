#include "input.c"

int main() {
	setRaw(1);
	char c;
	while(read(STDIN_FILENO, &c, 1) == 1) {
		if (c == 27) {
			break;
		} else {
			//printf("%c\n", c);
		}
	}
	/*
	int i = 0;
	while(true) {
		i = kbhit();
		if (i != 0) {
			char c = fgetc(stdin);
			if (c == 27) {
				break;
			}
		}
	}
	*/
	setRaw(0);
	return 0;
}
