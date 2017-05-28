#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//commands
int newNode(char *[], int, pid_t **, int **, int **,int);
void connect(char **, int *, int *);
void inject(char *[], int *);
void disconnect(char *[], int *);


void manInput(int);
void manOutput(void);
void getPipeName(char *,char *);
