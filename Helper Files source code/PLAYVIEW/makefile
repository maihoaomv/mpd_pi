# this is the command to run to to build executable  make clean && make
# make need to change the src path for main.c main.o
# change application name here (executable output name)
# SEE https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
# SEE https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Optimize-Options
TARGET=playview

# compiler
CC=gcc
# debug
DEBUG=-g0
# SETUP FOR DEBUGGING
#DEBUG=-ggdb
# optimisation
OPT=-O0
#OPT=-O10
# SETUP FOR DEBUGGING
#OPT=-Ofast
# warnings
#WARN=-Wall -Wextra -save-temps
WARN=-Wall -Wextra -Wunused -Wunreachable-code

#PTHREAD=-pthread

#CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe
CCFLAGS=$(DEBUG) $(OPT) $(WARN) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
#LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic -lmpdclient
LDFLAGS=$(GTKLIB) -export-dynamic -lmpdclient

OBJS= playview.o playview-resource.o
#OBJS= playview.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

playview.o: playview.c
	$(CC) -c $(CCFLAGS) $(WARN) playview.c $(GTKLIB) -o playview.o

playview-resource.o: playview-resource.c
	$(CC) -c $(CCFLAGS) $(WARN) playview-resource.c $(GTKLIB) -o playview-resource.o

clean:
	rm -f *.o $(TARGET)

version:
	touch playview.c
# auto generate version.h (overwrites existing version.h)
#	echo "#ifndef VERSION_STR" > playview_version.h
	echo "#define VERSION_STR \"`date +"%Y.%m.%d.%U.%j.%H.%M.%S"`\"" > playview_version.h
#	echo "#endif" >> playview_version.h
