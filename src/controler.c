#include<fcntl.h>
#include<signal.h>
#include<ctype.h>
#include "iStormAPI.h"
#define INITS 20

char **processCommand(char *, int *);

/*
* Controller which reads lines from stdin, parses them and send the result to the job handler for
* scheduling between nodes.
*/

int main(){
    pid_t *nodes = (pid_t*)calloc(INITS, sizeof(pid_t));
	int *status = (int*)calloc(INITS, sizeof(int)), input=0;
    int *pipes = (int*)calloc(INITS, sizeof(int)), len=0;
	char cmd[PIPE_BUF], **argv;
    int argc, nNodes=INITS, i; 
    int fd=open("log", O_CREAT, 0777);

    while(1){
        for(i = 0; read(0, cmd+i, 1) > 0 && *(cmd+i) != '\n'; input = input || (*(cmd+i) == ':'), i ++);
        if(input){
            cmd[i+1] = 0;
			for(i=0; i<nNodes; i++)
				if(status[i]) write(pipes[i],cmd,strlen(cmd));
            input = 0;
        }else{
            cmd[i] = 0;
            argv = processCommand(cmd, &argc);
            if(argc > 1){
                len = strlen(*argv);
                if(!strncmp(*argv, "node", (len < 4 ? len : 4))) nNodes = newNode(argv, argc, &nodes, &pipes, &status, nNodes);
                else if(!strncmp(*argv, "connect", (len < 7 ? len : 7))) connect(argv+1, pipes, status);
                else if(!strncmp(*argv, "inject", (len < 6 ? len : 6))) inject(argv, pipes);
                else if(!strncmp(*argv, "disconnect", (len < 10 ? len : 10))) disconnect(argv, pipes);            
                else if(!strncmp(*argv, "quit", (len < 4 ? len : 4))) exit(0);
            }    
            free(argv);
        }
    }
    free(status);
    free(nodes);
    return 0;
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

     args = (char**)calloc(words, sizeof(char*));
     *noArgs = i;

     i = 0;
     args[i] = strtok(command, " ");
     while(args[i]) args[++i] = strtok(NULL, " ");

     return args;
}
