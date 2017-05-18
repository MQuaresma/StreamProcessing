#include<fcntl.h>
#include<unistd.h>

/*
 *
 */
main(int argc, char *argv[]){

    int fd = open(argv[1], O_RDONLY); //open assigned pipe for communication with the controler
    int nPipe[2];
    char **args=NULL, *execN=NULL;
    pid_t proc=0;
    
    if(fd > 0){
        dup2(fd, 0); //stdin now becomes the assigned pipe 
        close(fd);
        args = (char**)calloc(argc-1, sizeof(void*));
        for(int i = 0; i < argc-1; args[i] = argv[i+2], i ++); //prepare arguments to be passed to execvp
    
        pipe(nPipe);

        if(!(proc = fork())){
            dup2(nPipe[0], 0);
            close(nPipe[1]);
            close(nPipe[0]);

            execN = (char*)calloc(3+strlen(argv[2], sizeof(char)));
            strcpy(exec, "./");
            strcat(exec, argv[2]);
            execvp(exec, args+2);
            perror("supervisor: execvp: ");
            _exit(1);
        }else{
            close(nPipe[1]);
            while(1){






            } 
        }
    }else perror("supervisor: ");
    
}

char *getPipeName(char *id){

    char *


}
