CC=gcc
#CFLAGS=
#LIBS=
DEP=commands.o controler.o supervisor.o
OBJ=window filter spawn const 

controler: $(OBJ) $(DEP)
	$(CC) -o indieStorm $(DEP)

controler.o: controler.c
	$(CC) -c controler.c

supervisor.o: supervisor.c
	$(CC) -c supervisor.c

commands.o: commands.c
	$(CC) -c commands.c

const: const.c
	$(CC) -o const const.c

filter: filter.c
	$(CC) -o filter filter.c

window: window.c
	$(CC) -o window window.c

spawn: spawn.c
	$(CC) -o spawn spawn.c

clean: 
	rm -rf const filter window spawn controler *.o
