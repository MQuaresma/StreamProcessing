#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define INITS 20
#define CONNECTIN ";c"
#define DCONNECTIN ";d"

int newNode(char **, int, pid_t**, int **, int);
void connect(char **, pid_t*);
char **readMessage(int argc);
char **processCommand(char *, int *);

/*
* Controller which reads lines from stdin, parses them and send the result to the job handler for
* scheduling between nodes.
*/

main(){
    pid_t *nodes = (pid_t*)calloc(INITS, sizeof(pid_t));
	short *status = (short*)calloc(INITS, sizeof(short)), input=0;
    char cmd[PIPE_BUF], **argv;
    int argc, nNodes=INITS;
    ssize_t r;

    while(1){
        for(int i = 0; read(0, cmd+i, 1) > 0 *(cmd+i) != '\n'; input = (*(cmd+i) == ':'), i ++);
        cmd[i] = 0;
        if(input){
            //send input to nodes
        }else{
            argv = processCommand(cmd, &argc);
            if(!strncmp(*argv, "node", 4)) nNodes = newNode(argv, argc1, &nodes, &status, nNodes);
            else if(!strncmp(*argv, "connect", 7)) connect(argv, nodes, status);
            else if(!strncmp(*argv, "inject", 6)) ;            
            free(argv);
        }
    }

    free(nodes);
}

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

/*
 * 
 *
 */
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

/*
 * Split a line into it's individual parameters
 * @param command Line to be parsed
 * @return Array of pointers to the different arguments
 */
char **processCommand(char *command, int *noArgs){

     char **args;
     int i, words;

     for(i = 0, words = 1; *(command+i); words += isspace(*(command+i)), i ++);

     args = (char**)calloc(words, sizeof(char));
     *noArgs = i;

     i = 0;
     args[i] = strtok(command, " ");
     while(args[i++]) args[i] = strtok(NULL, " ");

     return args;
}