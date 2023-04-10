# this is the command to run to to build executable  make clean && make
# make need to change the src path for main.c main.o
# change application name here (executable output name)
# SEE https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
# SEE https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Optimize-Options
#TARGET=mpd_pi

# compiler
CC=gcc
# debug
DEBUG=-g0
#DEBUG=-g3 -O0
#DEBUG=-g3
#OPT=-O0
OPT=-Ofast
# warnings
#WARN=-Wall -Wextra -save-temps
WARN=-Wall -Wextra -Wunused -Wunreachable-code

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=date && gcc
#LD=date && touch src/main.c && gcc
#LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic -lmpdclient
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

LDFLAGS	+= $(shell pkg-config --cflags --libs libmpdclient)
LDFLAGS	+= $(shell pkg-config --cflags --libs libnotify)


#COMPILE MULTIPLE PROGRAMS AT ONCE
#https://stackoverflow.com/questions/5950395/makefile-to-compile-multiple-c-programs

MPD_PI= object-files/main.o object-files/spawn.o object-files/popup_menu.o object-files/timer_handler.o object-files/other.o object-files/config.o object-files/getKeyVars.o object-files/keyboard.o object-files/songview.o object-files/trackview.o object-files/player.o object-files/playerState.o object-files/mouse.o object-files/mpdConnection.o object-files/signals.o object-files/getStatus.o object-files/statusDisplay.o object-files/mpd_pi-resource.o

all: $(MPD_PI)
	$(LD) -o ./mpd_pi $(MPD_PI) $(LDFLAGS)

object-files/main.o: src/main.c
	$(CC) -c $(CCFLAGS) $(WARN) src/main.c $(GTKLIB) -o object-files/main.o

object-files/spawn.o: src/spawn.c
	$(CC) -c $(CCFLAGS) $(WARN) src/spawn.c $(GTKLIB) -o object-files/spawn.o

object-files/popup_menu.o: src/popup_menu.c
	$(CC) -c $(CCFLAGS) $(WARN) src/popup_menu.c $(GTKLIB) -o object-files/popup_menu.o

object-files/timer_handler.o: src/timer_handler.c
	$(CC) -c $(CCFLAGS) $(WARN) src/timer_handler.c $(GTKLIB) -o object-files/timer_handler.o

object-files/other.o: src/other.c
	$(CC) -c $(CCFLAGS) $(WARN) src/other.c $(GTKLIB) -o object-files/other.o

object-files/config.o: src/config.c
	$(CC) -c $(CCFLAGS) $(WARN) src/config.c $(GTKLIB) -o object-files/config.o

object-files/getKeyVars.o: src/getKeyVars.c
	$(CC) -c $(CCFLAGS) $(WARN) src/getKeyVars.c $(GTKLIB) -o object-files/getKeyVars.o

object-files/keyboard.o: src/keyboard.c
	$(CC) -c $(CCFLAGS) $(WARN) src/keyboard.c $(GTKLIB) -o object-files/keyboard.o

object-files/songview.o: src/songview.c
	$(CC) -c $(CCFLAGS) $(WARN) src/songview.c $(GTKLIB) -o object-files/songview.o


object-files/trackview.o: src/trackview.c
	$(CC) -c $(CCFLAGS) $(WARN) src/trackview.c $(GTKLIB) -o object-files/trackview.o

object-files/player.o: src/player.c
	$(CC) -c $(CCFLAGS) $(WARN) src/player.c $(GTKLIB) -o object-files/player.o

object-files/playerState.o: src/playerState.c
	$(CC) -c $(CCFLAGS) $(WARN) src/playerState.c $(GTKLIB) -o object-files/playerState.o

object-files/mouse.o: src/mouse.c
	$(CC) -c $(CCFLAGS) $(WARN) src/mouse.c $(GTKLIB) -o object-files/mouse.o

object-files/mpdConnection.o: src/mpdConnection.c
	$(CC) -c $(CCFLAGS) $(WARN) src/mpdConnection.c $(GTKLIB) -o object-files/mpdConnection.o

object-files/signals.o: src/signals.c
	$(CC) -c $(CCFLAGS) $(WARN) src/signals.c $(GTKLIB) -o object-files/signals.o

object-files/getStatus.o: src/getStatus.c
	$(CC) -c $(CCFLAGS) $(WARN) src/getStatus.c $(GTKLIB) -o object-files/getStatus.o

object-files/statusDisplay.o: src/statusDisplay.c
	$(CC) -c $(CCFLAGS) $(WARN) src/statusDisplay.c $(GTKLIB) -o object-files/statusDisplay.o

object-files/mpd_pi-resource.o: src/mpd_pi-resource.c
	$(CC) -c $(CCFLAGS) $(WARN) src/mpd_pi-resource.c $(GTKLIB) -o object-files/mpd_pi-resource.o

clean:
	rm -f object-files/*.o mpd_pi
