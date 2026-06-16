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

prod: CFLAGS += $(PROD_CFLAGS)
prod: LDFLAGS += $(PROD_LDFLAGS)
prod: $(TARGET)

FD = Form/

# Linking
$(TARGET): helper.h libCore.a libForm.a libInput.a libHelper.a  main.o  
	gcc main.o -o $@ $(LDFLAGS) libForm.a libInput.a libCore.a libHelper.a

libHelper.a:
	$(MAKE) -C ../FormNetwork/
	cp ../FormNetwork/libHelper.a .

helper.h:
	$(MAKE) -C ../FormNetwork/
	cp ../FormNetwork/helper.h .


# Static lib
libForm.a: form.o cell.o world.o game.o  
	ar rs $@ $^

libInput.a: input.o output.o renderFrame.o
	ar rs $@ $^

libCore.a: core.o threads.o timeWizard.o poll.o
	ar rs $@ $^

# Compiling
main.o: main.c
	gcc $(CFLAGS) -c main.c -o $@

output.o: output.c output.h
	gcc $(CFLAGS) -c output.c -o $@

renderFrame.o: renderFrame.c renderFrame.h
	gcc $(CFLAGS) -c renderFrame.c -o $@

input.o: input.c input.h
	gcc $(CFLAGS) -c input.c -o $@


# FORM
game.o: $(FD)game.c $(FD)game.h
	gcc $(CFLAGS) -c $(FD)game.c -o $@

form.o: $(FD)form.c $(FD)form.h
	gcc $(CFLAGS) -c $(FD)form.c -o $@

cell.o: $(FD)cell.c $(FD)cell.h
	gcc $(CFLAGS) -c $(FD)cell.c -o $@

world.o: $(FD)world.c $(FD)world.h
	gcc $(CFLAGS) -c $(FD)world.c -o $@

core.o: core.h core.c helper.h
	gcc $(CFLAGS) -c core.c -o $@

poll.o: poll.c poll.h
	gcc $(CFLAGS) -c poll.c -o $@

timeWizard.o: timeWizard.c timeWizard.h
	gcc $(CFLAGS) -c timeWizard.c -o $@

threads.o: threads.c threads.h
	gcc $(CFLAGS) -c threads.c -o $@



# tools
clean:
	rm -f *.o *.a *.d

fclean:
	rm -f $(TARGET) *.o *.a *.d helper.h

fixTerminal:
	stty sane

# merges .d files into dependency graph
-include *.d
