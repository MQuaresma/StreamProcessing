#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/syslimits.h>

char **processCommand(char *command);
/*
 * Controller which reads lines from stdin, parses them and send the result to the job handler for 
 * scheduling between nodes.
 */
main(){
    
    char command[PIPE_BUF], **args = NULL;
    int i, controlerTracker[2];
    pid_t jobTracker;

    // Establish a way of communication between job tracker and controler
    if(pipe(controlerTracker) != -1){
        if(!(jobTracker=fork())){
            dup2(controlerTracker[0], 0); //stdin becomes output of pipe
            close(controlerTracker[0]);
            close(controlerTracker[1]); //close input end to avoid conflicts
            printf("Dummy print\n");
            exit(0);
            //job tracker code
        }else{
            close(controlerTracker[0]);
            while(1){
                for(i = 0; i < PIPE_BUF && read(0, command+i, 1) > 0 && *(command+i)!='\n'; i ++);
                command[i] = 0;
                args = processCommand(command);
                for(i = 0; args[i]; printf("%s\n", args[i]), i ++);
                //for(i = 0; args[i]; i ++) write(controlerTracker[1], args[i], strlen(args[i]));
                //signal finish of command?
                //pipe command to the job handler
                free(args);
            }
        }
    }else fprintf(stderr, "Couldn't reach job tracker process, exiting...\n");
}

/*
 * Split a line into it's individual parameters
 * @param command Line to be parsed
 * @return Array of pointers to the different arguments
*/
char **processCommand(char *command){

    char **args;
    int i, words;

    for(i = 0, words = 1; *(command+i); words += isspace(*(command+i)), i ++);

    args = (char**)calloc(words, sizeof(char));

    i = 0;
    args[i] = strtok(command, " ");
    while(args[i++]) args[i] = strtok(NULL, " ");

    return args;

}