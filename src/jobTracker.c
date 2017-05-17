#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#define INITS 20

int newNode(char **, int, pid_t**, int **, int);
void connect(char **, pid_t*);
char **readMessage(int argc);

void jobTracker(){

    int argc, nNodes=INITS;
    ssize_t r;
    char cmd[INITS], **argv;
    pid_t *nodes = (pid_t*)calloc(INITS, sizeof(pid_t));

    signal(SIGUSR1, recieveMesssage);

    while(1){
        pause();
        r = read(0, cmd, INITS);
        cmd[r] = 0;
        argc = atoi(cmd);
        r = read(0, cmd, INITS); //read command to be executed
        if(r > 0){
            argv = readMessage(argc);
            if(!strncmp(cmd, "node", r)) nNodes = newNode(argv, argc, &nodes, nNodes);
            else if(!strncmp(cmd, "connect", r)) connect(argv, nodes);

            for(int i = 0; argv[i]; i ++) free(argv[i]);
            free(argv);
        }
    }

    free(nodes);

}

void recieveMesssage(int sig){

   if(sig == SIGUSR2) kill(getpid(), SIGINT);

}

int newNode(char *args[], int argc, pid_t **nodes, int **pipes, int nds){
	int id, i, p, pf[2];
	
	id = atoi(args[0]);
	pipe(pf);
	
	char **ar = (char**)calloc(argc+1,sizeof(char*));
	ar[0]="supervisor";
	
    for(i=1; args[i]; i++) ar[i]=args[i];

	if((p=fork())==0){
			close(pf[1]);
			dup2(pf[0],0);
			close(pf[0]);
			execvp("./supervisor",ar);
			perror("jobTracker: supervisor: execvp: ");
			_exit(1);
	}
	close(pf[0]);

	if(id>=nds){
		nds = nds + nds/2;
		*nodes = realloc(*nodes,nds);
		*pipes = realloc(*pipes,nds);
	}

    free(ar);
	(*nodes)[id]= p;
	(*pipes)[id]= pf[1];
	return nds;			
}

char **readMessage(int argc){
    char **res = (char**)calloc(argc, sizeof(char*)), buf[INITS];

    for(int i = 0; i < n-1 && (r = read(0, buf, INITS))>0; i ++){
        buf[r] = 0;
        res[i] = strndup(buf, r); 
    }

   return res; 
}

void connect(char **cmd, pid_t *nodes, int *pd){
    pid_t src, dest;
    char pipeName[10]={0};
    int len;

    if(*cmd){
        src = atoi(*cmd); //id of the source node
        len = strlen(src);
        pipeName[len] = 0;
        strcat(pipeName, src);
        strcat(pipeName, "-");
        len ++;
        while(*++cmd){
            pipeName[len] = 0;
            dest = atoi(*cmd); //get index of destination of output
            strcat(pipeName, *cmd);  //name of the named pipe
            kill(nodes[src], SIGUSR1); //warn src node to add a new output to it's list
            write(pd[src], pipeName, strlen(pipeName));
            kill(nodes[dest], SIGUSR2); //warn dest node that it's input has changed
            write(pd[dest], pipeName, strlen(pipeName));
        }    
    }
}
