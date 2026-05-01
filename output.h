#ifndef OUTPUT
#define OUTPUT

#include <signal.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdatomic.h>

extern atomic_int windowResized;

void initScreen();
void updateScreen();
void windowResizeCallback(int sig);
void makeScreen();

#endif
