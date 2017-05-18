#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#define INITS 20
#define CONNECTIN ";c"
#define DCONNECTIN ";d"

int newNode(char **, int, pid_t**, int **, int);
void connect(char **, pid_t*);
char **readMessage(int argc);

void jobTracker(){
    int argc, nNodes=INITS;
    ssize_t r;
    char cmd[INITS], **argv;
    pid_t *nodes = (pid_t*)calloc(INITS, sizeof(pid_t));
	short *status = (short*)calloc(INITS, sizeof(short));

    while(1){
        pause();
        r = read(0, cmd, INITS);
        cmd[r] = 0;
        argc = atoi(cmd);
        r = read(0, cmd, INITS); //read command to be executed
        if(r > 0){
            argv = readMessage(argc);
            if(!strncmp(cmd, "node", r)) nNodes = newNode(argv, argc, &nodes, &status, nNodes);
            else if(!strncmp(cmd, "connect", r)) connect(argv, nodes, status);

            for(int i = 0; argv[i]; i ++) free(argv[i]);
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

char **readMessage(int argc){
    char **res = (char**)calloc(argc, sizeof(void*)), buf[INITS];

    for(int i = 0; i < n-1 && (r = read(0, buf, INITS))>0; i ++){
        buf[r] = 0;
        res[i] = strndup(buf, r); 
    }

   return res; 
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
