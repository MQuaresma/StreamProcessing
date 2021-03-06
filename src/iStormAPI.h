#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct statusNode{
     int nd;
     struct statusNode *prox;
}*statusNodeP;

//commands
int newNode(char *[], int, pid_t **, int **, statusNodeP **,int *);
void connect(char **, int *, statusNodeP*);
void inject(char *[], int *);
void disconnect(char *[], int *, statusNodeP*);
void removeNode(char *[],statusNodeP*, pid_t *, int *, int, int);

void manInput(int);
void manOutput(pid_t);
void getPipeName(char *,char *);
