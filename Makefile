DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

PROD_CFLAGS = -O2
PROD_LDFLAGS =

dev: main.c input.c input.h
	gcc main.c -o inputThread $(DEV_CFLAGS) $(DEV_LDFLAGS)

prod: main.c input.c input.h
	gcc main.c -o inputThread $(PROD_CFLAGS) $(PROD_LDFLAGS)

clean:
	rm -f inputThread

fixTerminal:
	stty sane
