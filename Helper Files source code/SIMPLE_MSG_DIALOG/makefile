# this is the command to run to to build executable  make clean && make
# make need to change the src path for main.c main.o
# change application name here (executable output name)
# SEE https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html
# SEE https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Optimize-Options
TARGET=simpleMsgDialog

# compiler
CC=date && gcc   #adding date to gcc will cause make to print date and time of compile to the command line
# debug
DEBUG=-g0
#DEBUG=-g3
# SETUP FOR DEBUGGING
#DEBUG=-g
# optimisation
#OPT=-O0
#OPT=-O10
# SETUP FOR DEBUGGING
#OPT=-Ofast
# warnings
#WARN=-Wall -Wextra -save-temps
WARN=-Wall -Wextra -Wunused -Wunreachable-code

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=date && gcc

LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS= simpleMsgDialog.o simpleMsgDialog-resource.o

all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

simpleMsgDialog.o: simpleMsgDialog.c
	$(CC) -c $(CCFLAGS) $(WARN) simpleMsgDialog.c $(GTKLIB) -o simpleMsgDialog.o

simpleMsgDialog-resource.o: simpleMsgDialog-resource.c
	$(CC) -c $(CCFLAGS) $(WARN) simpleMsgDialog-resource.c $(GTKLIB) -o simpleMsgDialog-resource.o

clean:
	rm -f *.o $(TARGET)
