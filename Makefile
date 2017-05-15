CC=gcc
#CFLAGS=
#LIBS=
DEP=controler.o jobTracker.o supervisor.o
OBJ= window filter spawn const 

controler: $(OBJ) $(DEP)
	$(CC) -o controler $(DEP)

controler.o: controler.c
	$(CC) -c controler.c

supervisor.o: supervisor.c
	$(CC) -c supervisor.c

jobTracker.o: jobTracker.c
	$(CC) -c jobTracker.c

const: const.c
	$(CC) -o const const.c

filter: filter.c
	$(CC) -o filter filter.c

window: window.c
	$(CC) -o window window.c

spawn: spawn.c
	$(CC) -o spawn spawn.c