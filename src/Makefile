CC=gcc
CFLAGS=-g -Wall
#LIBS=
DEP=commands.o controler.o
OBJ=window filter spawn const supervisor

indieStorm: $(OBJ) $(DEP)
	$(CC) $(CFLAGS) -o indieStorm $(DEP)

controler.o: controler.c
	$(CC) $(CFLAGS) -c controler.c

supervisor: supervisor.c
	$(CC) $(CFLAGS) -o supervisor supervisor.c

commands.o: commands.c
	$(CC) $(CFLAGS) -c commands.c

const: const.c
	$(CC) $(CFLAGS) -o const const.c

filter: filter.c
	$(CC) $(CFLAGS) -o filter filter.c

window: window.c
	$(CC) $(CFLAGS) -o window window.c

spawn: spawn.c
	$(CC) $(CFLAGS) -o spawn spawn.c

clean: 
	rm -rf const filter window spawn indieStorm supervisor *.o
