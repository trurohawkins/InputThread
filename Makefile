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

CD = Core/
GD = Game/
FD = Form/

# Linking
$(TARGET): helper.h libForm.a libGame.a libInput.a  libCore.a libHelper.a  main.o  
	gcc main.o -o $@ $(LDFLAGS) libForm.a libGame.a libInput.a libCore.a libHelper.a -lm

libHelper.a:
	$(MAKE) -C ../FormNetwork/
	cp ../FormNetwork/libHelper.a .

helper.h:
	$(MAKE) -C ../FormNetwork/
	cp ../FormNetwork/helper.h .


# Static lib
libForm.a: form.o cell.o world.o WorldManager.o  
	ar rs $@ $^

libGame.a: game.o timeWizard.o renderFrame.o player.o inputMap.o   
	ar rs $@ $^

libInput.a: input.o output.o keys.o 
	ar rs $@ $^

libCore.a: core.o threads.o  poll.o
	ar rs $@ $^

# Compiling
main.o: main.c
	gcc $(CFLAGS) -c main.c -o $@

output.o: output.c output.h
	gcc $(CFLAGS) -c output.c -o $@

# Terminal input
input.o: input.c input.h
	gcc $(CFLAGS) -c input.c -o $@

keys.o: keys.c keys.h
	gcc $(CFLAGS) -c keys.c -o $@

# FORM
form.o: $(FD)form.c $(FD)form.h
	gcc $(CFLAGS) -c $(FD)form.c -o $@

cell.o: $(FD)cell.c $(FD)cell.h
	gcc $(CFLAGS) -c $(FD)cell.c -o $@

world.o: $(FD)world.c $(FD)world.h
	gcc $(CFLAGS) -c $(FD)world.c -o $@

WorldManager.o: $(FD)WorldManager.c $(FD)WorldManager.h
	gcc $(CFLAGS) -c $(FD)WorldManager.c -o $@

#GAME
game.o: $(GD)game.c $(GD)game.h
	gcc $(CFLAGS) -c $(GD)game.c -o $@

renderFrame.o: $(GD)renderFrame.c $(GD)renderFrame.h
	gcc $(CFLAGS) -c $(GD)renderFrame.c -o $@

timeWizard.o: $(GD)timeWizard.c $(GD)timeWizard.h
	gcc $(CFLAGS) -c $(GD)timeWizard.c -o $@

player.o: $(GD)player.c $(GD)player.h $(GD)playerManager.c $(GD)playerManager.h
	gcc $(CFLAGS) -c $(GD)player.c -o $@

inputMap.o: $(GD)inputMap.c $(GD)inputMap.h
	gcc $(CFLAGS) -c $(GD)inputMap.c -o $@

#CORE
core.o: $(CD)core.h $(CD)core.c helper.h
	gcc $(CFLAGS) -c $(CD)core.c -o $@

poll.o: $(CD)poll.c $(CD)poll.h
	gcc $(CFLAGS) -c $(CD)poll.c -o $@

threads.o: $(CD)threads.c $(CD)threads.h
	gcc $(CFLAGS) -c $(CD)threads.c -o $@



# tools
clean:
	rm -f *.o *.a *.d

fclean:
	rm -f $(TARGET) *.o *.a *.d helper.h

fixTerminal:
	stty sane

# merges .d files into dependency graph
-include *.d
