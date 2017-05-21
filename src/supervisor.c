#include<fcntl.h>
#include<unistd.h>
#define INITS 20

/*
 *
 */
main(int argc, char *argv[]){

    int fd = open(argv[1], O_RDONLY); //open assigned pipe for communication with the controler
    int nPipeIN[2], nPipeOUT[2];
    char **args=NULL, *execN=NULL;
    pid_t proc=0;
    
    if(fd > 0){
        args = (char**)calloc(argc-1, sizeof(void*));
        for(int i = 0; i < argc-1; args[i] = argv[i+2], i ++); //prepare arguments to be passed to execvp
    
        pipe(nPipeIN);
        pipe(nPipeOUT);

        if(!(proc = fork())){
            dup2(nPipeIN[0], 0);
            dup2(nPipeOUT[1], 1);
            close(nPipeIN[1]);
            close(nPipeIN[0]);
            close(nPipeOUT[1]);
            close(nPipeOUT[0]);
            close(fd);
            execN = (char*)calloc(3+strlen(argv[2]), sizeof(char));
            strcpy(execN, "./");
            strcat(execN, args+2);
            execvp(execN, args+2);
            perror("supervisor: execvp: ");
            _exit(1);
        }else{
            close(nPipeIN[0]);
            close(nPipeOUT[1]);
            if(!fork()){ 
                dup2(fd, 0);     
                dup2(nPipeIN[1], 1);
                close(nPipeIN[1]);
                close(nPipeIN[0]);
                close(nPipeOUT[1]);
                close(nPipeOUT[0]);
                close(fd);
                manInput();
            }
            if(!fork()){
                dup2(nPIPEOUT[0], 0); 
                close(nPipeIN[1]);
                close(nPipeIN[0]);
                close(nPipeOUT[1]);
                close(nPipeOUT[0]);
                close(fd);
                manOutput();
            }
            wait(NULL);
        }
    }else perror("supervisor: ");
    
}


void manInput(void){




}


void manOutput(void){

    int *pipes = (int*)calloc(INITS, sizeof(int));


}
