#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#define INITS 20

char **readMessage(int argc);
char **processCommand(char *, int *);

/*
* Controller which reads lines from stdin, parses them and send the result to the job handler for
* scheduling between nodes.
*/

main(){
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

char **readMessage(int argc){
    char **res = (char**)calloc(argc, sizeof(void*)), buf[INITS];

    for(int i = 0; i < n-1 && (r = read(0, buf, INITS))>0; i ++){
        buf[r] = 0;
        res[i] = strndup(buf, r); 
    }

   return res; 
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

