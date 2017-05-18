#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#define CONNECTIN ";c"
#define DCONNECTIN ";d"

int newNode(char *args[], int argc, pid_t **nodes, int **pipes, short **status,int nds){
	int id, i, p, f, fd;

	char name[strlen(args[0])+4];
	name[0]='n';
	name[1]='o';
	name[2]='d';
	name[3]='e';
	strcat(name,args[0]);
	f = mkfifo(name, 0666);

	id = atoi(args[0]);

	if(f>0){
		fd = open(name,O_WRONLY);

		char **ar = (char**)calloc(argc+1,sizeof(void*));
		ar[0]="supervisor";

		for(i=1; args[i]; i++) ar[i]=args[i];

		if((p=fork())==0){
			execvp("./supervisor",ar);
			perror("jobTracker: supervisor: execvp: ");
			_exit(1);
		}

		if(id>=nds){
			nds = nds + nds/2;
			*nodes = (pid_t*)realloc(*nodes,nds);
			*pipes = (int*)realloc(*pipes,nds);
			*status = (short*)realloc(*status,nds);
		}

		free(ar);
		(*nodes)[id] = p;
		(*pipes)[id] = fd;
        (*pipes)[id] = 1;
    }else nds=-1;
	return nds;
}

void connect(char **cmd, pid_t *nodes, int *pd, short *status){
	pid_t dest;
	char pipeName[strlen(*cmd)+3]={0};

    if(*cmd){
    	// commands from the jobTracker are delimited by semi-colons
	    strcat(pipeName, CONNECTIN);
	    pipeName[3] = 0;
        strcat(pipeName, *cmd);
	    strcat(pipeName, ";");
        while(*++cmd){
			dest = atoi(*cmd);
			status[dest] = 0;
            write(pd[dest], pipeName, strlen(*cmd)+3);
        }
    }else fprintf(stderr, "jobTracker: connect: no nodes specified");
}