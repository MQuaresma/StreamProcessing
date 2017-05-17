#include <unistd.h>
#include <strings.h>
#include <signals.h>
#define INITS 20


void newNode(int, pid_t*);
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
            if(!strncmp(cmd, "node", r)) newNode(argc, nodes);
            else if(!strncmp(cmd, "connect", r))
                if(connect(argc, nodes)) fprintf(stderr, "Couldn't connect request nodes\n");
        }
    }

    free(nodes);

}

void recieveMesssage(int sig){

   if(sig == SIGUSR2) kill(getpid(), SIGINT);

}
