#include <unistd.h>
#include <strings.h>
#include <signals.h>
#define INITS 20

int newNode(char **, pid_t **, int);
int connect(int, pid_t*);


void jobTracker(){

    int argc;
    ssize_t r;
    char cmd[INITS];
    pid_t *nodes = (pid_t*)calloc(INITS, sizeof(pid_t));

    signal(SIGUSR1, recieveMesssage);

    while(1){
        pause();
        r = read(0, cmd, INITS);
        cmd[r] = 0;
        argc = atoi(cmd);
        r = read(0, cmd, INITS); //read command to be executed
        if(r > 0){
            if(!strncmp(cmd, "node", r)) newNode(cmd, nodes);
            else if(!strncmp(cmd, "connect", r))
                if(connect(argc, nodes)) fprintf(stderr, "Couldn't connect request nodes\n");
        }
    }

    free(nodes);

}

void recieveMesssage(int sig){

   if(sig == SIGUSR2) kill(getpid(), SIGINT);

}

int newNode(char *args[], pid_t **nodes, int **pipes, int nds){
	int id, i, p, space=0, pf[2];
	
	id = atoi(args[0]);
	pipe(pf);
	
	for(i=0;args[i]; i++);
	i++;
	char **ar == (char**)calloc(i,sizeof(char));
	
	ar[0]="supervisor";
	for(i=1; args[i]; i++) ar[i]=strdup(args[i]);

	if((p=fork())==0){
			close(pf[1]);
			dup2(pf[0],0);
			close(pf[0]);
			execvp("./supervisor",ar);
			perror("Supervisor Error");
			_exit(1);
	}
	close(pf[0]);	

	if(id==nds){
		nds = nds + nds/2;
		*nodes = realloc(*nodes,nds);
		*pipes = realloc(*pipes,nds);
	}
	(*nodes)[id]= p;
	(*pipes)[id]= pf[1];
	return nds;			
}
