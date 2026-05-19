TARGET = inputThread

DEV_CFLAGS = -g -fsanitize=address,undefined -fno-omit-frame-pointer
DEV_LDFLAGS = -fsanitize=address,undefined

TSAN_CFLAGS = -g -O1 -fsanitize=thread -fno-omit-frame-pointer
TSAN_LDFLAGS = -fsanitize=thread

PROD_CFLAGS = -O2
PROD_LDFLAGS =

CFLAGS = -MMD -MP
LDFLAGS =

dev: CFLAGS += $(DEV_CFLAGS)
dev: LDFLAGS += $(DEV_LDFLAGS)
dev: $(TARGET)

tsan: CFLAGS += $(TSAN_CFLAGS)
tsan: LDFLAGS += $(TSAN_LDFLAGS)
tsan: $(TARGET)

prod: CFLAGS += $(PROD_FLAGS)
prod: LDFLAGS += $(DEV_LDFLAGS)
prod: $(TARGET)

# Linking
$(TARGET): main.o libInput.a libHelper.a
	gcc main.o -o $@ $(LDFLAGS) libInput.a libHelper.a

libHelper.a:
	$(MAKE) -C ../FormNetwork/
	cp ../FormNetwork/libHelper.a .

helper.h:
	$(MAKE) -C ../FormNetwork/
	cp ../FormNetwork/helper.h .


# Static lib
libInput.a: input.o output.o core.o threads.o
	ar rs $@ $^

# Compiling
main.o: main.c
	gcc $(CFLAGS) -c main.c -o $@

input.o: input.c input.h
	gcc $(CFLAGS) -c input.c -o $@

output.o: output.c output.h
	gcc $(CFLAGS) -c output.c -o $@

core.o: core.h core.c game.c game.h helper.h
	gcc $(CFLAGS) -c core.c -o $@

threads.o: threads.c threads.h
	gcc $(CFLAGS) -c threads.c -o $@



# tools
clean:
	rm -f *.o *.a *.d

fclean:
	rm -f $(TARGET) *.o *.a *.d

fixTerminal:
	stty sane

# merges .d files into dependency graph
-include *.d
