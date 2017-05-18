#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define INITS 20

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