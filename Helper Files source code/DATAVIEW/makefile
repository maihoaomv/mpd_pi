# this is the command to run to to build executable  make clean && make
# make need to change the src path for main.c main.o
# change application name here (executable output name)
# SEE https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
# SEE https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Optimize-Options
TARGET=dataview

# compiler
CC=gcc
# debug
#DEBUG=-g0
# SETUP FOR DEBUGGING
#DEBUG=-ggdb
# optimisation
#OPT=-O0
OPT=-O10
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

OBJS= dataview.o dataview-resource.o
#OBJS= dataview.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

dataview.o: dataview.c
	$(CC) -c $(CCFLAGS) $(WARN) dataview.c $(GTKLIB) -o dataview.o

dataview-resource.o: dataview-resource.c
	$(CC) -c $(CCFLAGS) $(WARN) dataview-resource.c $(GTKLIB) -o dataview-resource.o

clean:
	rm -f *.o $(TARGET)

version:
	touch dataview.c
# auto generate version.h (overwrites existing version.h)
#	echo "#ifndef VERSION_STR" > dataview_version.h
	echo "#define VERSION_STR \"`date +"%Y.%m.%d.%U.%j.%H.%M.%S"`\"" > dataview_version.h
#	echo "#endif" >> dataview_version.h
