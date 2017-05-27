#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//commands
int newNode(char *[], int, pid_t **, int **, short **,int);
void connect(char **, pid_t *, int *, short *);
void inject(char *[], int *);
void disconnect(char *[], int *);
