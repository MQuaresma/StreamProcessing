#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include "iStormAPI.h"
#define CONNECTIN ";c"
#define DCONNECTIN ";d"

int newNode(char *args[], int argc, pid_t **nodes, int **pipes, statusNodeP **status,int *nds){
	int id, p, f, fd, ret=0;

	char name[strlen(args[1])+4];
    if(argc>1){
        name[0]='n';
        name[1]='o';
        name[2]='d';
        name[3]='e';
        name[4]=0;
        strcat(name,args[1]);
        f=mkfifo(name, 0666);
        id = atoi(args[1]);

        if(!f){
            args[0]="supervisor";
            args[1]=name;
            if((p=fork())==0){
                execvp("./supervisor",args);
                perror("commands: newNode: execvp: ");
                _exit(1);
            }
            if(id>=*nds){
                *nds = *nds + *nds/2;
                *nodes = (pid_t*)realloc(*nodes,*nds);
                *pipes = (int*)realloc(*pipes,*nds); 
                *status = (statusNodeP*)realloc(*status,*nds);
            }
            fd = open(name,O_WRONLY);
            if(fd>0){
                (*nodes)[id] = p;
                (*pipes)[id] = fd;
            }else {
                perror("newNode: Cannot open file");
                ret=1;
            }   
        }else{
            perror("commands:");
            ret=1;
        }
    }else {
        fprintf(stderr, "commands: newNode: Not enough arguments");
        ret=1;
    }
	return ret;
}

/*
 * Connects one node's output to the input of certain amount of nodes
 */
void connect(char **cmd, int *pd, statusNodeP *status){
	pid_t dest, src;
    
    if(*cmd){
    	// commands from the jobTracker are delimited by semi-colons
        src = atoi(*cmd);
        while(*++cmd){
            char *pipeName = (char*)calloc(strlen(*cmd)+4, sizeof(char));
            strcat(pipeName, CONNECTIN);
            dest = atoi(*cmd);
            statusNodeP aux = (statusNodeP)malloc(sizeof(struct statusNode));
            aux->nd=src;
            aux->prox=status[dest];
            status[dest] = aux;
            pipeName[2] = 0;
            strcat(pipeName, *cmd);
            strcat(pipeName, "\n");
            write(pd[src], pipeName, strlen(*cmd)+3);
            free(pipeName);
        }
    }else fprintf(stderr, "commands: connect: no nodes specified");
}

/*
 * Injects the output of executing the command given into the node
 * specified
 */
void inject(char *args[], int *pipes){
	int id;
    
    id = atoi(args[1]);

	if(fork()==0){
		dup2(pipes[id],1);
		execvp(args[2], args+2);
		perror("controler: inject: ");
		_exit(1);
	}
    wait(NULL);
} 

/*
 * Disconnects two nodes 
 */
void disconnect(char *args[], int *pipes, statusNodeP *status){
	int id1, id2, rem=0;
	char *pipeName = (char*)calloc(strlen(*args)+3, sizeof(char));
    statusNodeP aux = NULL, prev=NULL;

	id1 = atoi(args[1]);
	id2 = atoi(args[2]);
	strcat(pipeName, DCONNECTIN);
	pipeName[3] = 0;
	strcat(pipeName, *(args+2));
	strcat(pipeName, "\n");
    
    for(aux=status[id2];!rem && aux; prev=aux, aux=aux->prox){
        if((rem=aux->nd==id1)){
            if(!prev) status[id2] = aux->prox;
            else prev->prox=aux->prox;
            free(aux);
        }
    }

	write(pipes[id1], pipeName, strlen(pipeName)); //remove id2 from the nodes that id1 outputs to
    free(pipeName);
}

void removeNode(char *args[], statusNodeP *status, int *pipes, int *activeNodes){
    int nd=atoi(args[1]); 
    int *dest=(int*)calloc(activeNodes,sizeof(int));
  
    free(dest); 
}
