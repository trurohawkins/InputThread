#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

void setRaw(int state);
int kbhit();
