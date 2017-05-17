#include <unistd.h>
#include <strings.h>
#include <signals.h>
#define INITS 20


void jobTracker(){

    int argc;
    pid_t *nodes = (pid_t*)calloc(INITS, sizeof(pid_t));
    char buf[PIPE_BUF], noArgs[10], **argv;
    ssize_t r;

    signal(SIGUSR1, recieveMesssage);

    while(1){
        pause();
        r = read(0, noArgs, 10);
        noArgs[r] = 0;
        argc = atoi(noArgs);
        argv = (char**)calloc(argc, sizeof(void*));
        for(int i = 0; i < argc && (r = read(1, argv[i])); i ++){
            if(!strncmp(buf, "node", r)){
                while((r = read(0, buf, PIPE_BUF)) > 0 && buf[0] != ';')
                    sscanf("%d", )
                    nodes[fpr]

            }


        }
         

        free(argv);
    }

    free(nodes);

}

void recieveMesssage(int sig){

   if(sig == SIGUSR2) kill(getpid(), SIGINT);

}
