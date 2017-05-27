#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include "iStormAPI.h"
#define CONNECTIN ";c"
#define DCONNECTIN ";d"

int newNode(char *args[], int argc, pid_t **nodes, int **pipes, short **status,int nds){
	int id, p, f, fd;

	char name[strlen(args[1])+4];
	name[0]='n';
	name[1]='o';
	name[2]='d';
	name[3]='e';
	strcat(name,args[1]);
	f = mkfifo(name, 0666);

	id = atoi(args[1]);

	if(f>0){
		fd = open(name,O_WRONLY);
		args[0]="supervisor";
		args[1]=name;

		if((p=fork())==0){
			execvp("./supervisor",args);
			perror("commands: newNode: execvp: ");
			_exit(1);
		}

		if(id>=nds){
			nds = nds + nds/2;
			*nodes = (pid_t*)realloc(*nodes,nds);
			*pipes = (int*)realloc(*pipes,nds);
			*status = (short*)realloc(*status,nds);
		}

		(*nodes)[id] = p;
		(*pipes)[id] = fd;
        (*pipes)[id] = 1;
    }else nds=-1;
	return nds;
}

/*
 * Connects one node's output to the input of certain amount of nodes
 */
void connect(char **cmd, pid_t *nodes, int *pd, short *status){
	pid_t dest;
	char *pipeName = (char*)calloc(strlen(*cmd)+3, sizeof(char));

    if(*cmd){
    	// commands from the jobTracker are delimited by semi-colons
	    strcat(pipeName, CONNECTIN);
	    pipeName[3] = 0;
        strcat(pipeName, *cmd);
	    strcat(pipeName, "\n");
        while(*++cmd){
			dest = atoi(*cmd);
			status[dest] = 0;
            write(pd[dest], pipeName, strlen(*cmd)+3);
        }
    }else fprintf(stderr, "commands: connect: no nodes specified");
    free(pipeName);
}

/*
 * Injects the output of executing the command given into the node
 * specified
 */
void inject(char *args[], int *pipes){
	int id, pf[2];
    
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
void disconnect(char *args[], int *pipes){
	int id1, id2;
	char *pipeName = (char*)calloc(strlen(*args)+2, sizeof(char));

	id1 = atoi(args[1]);
	id2 = atoi(args[2]);
	strcat(pipeName, DCONNECTIN);
	pipeName[3] = 0;
	strcat(pipeName, *(args+2));
	strcat(pipeName, "\n");

	pipes[id2] = 0; //change source of id2

	write(pipes[id1], pipeName, strlen(pipeName)); //remove id2 from the nodes that id1 outputs to
    free(pipeName);
}
