#include<fcntl.h>
#include "iStormAPI.h"
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
            if(!fork()){ 
                dup2(fd, 0);     
                dup2(nPipeIN[1], 1);
                close(nPipeIN[1]);
                close(nPipeIN[0]);
                close(nPipeOUT[1]);
                close(nPipeOUT[0]);
                close(fd);
                manInput(nPIPEOUT[1]);
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
            close(nPipeIN[0]);
            close(nPipeIN[1]);
            close(nPipeOUT[1]);
            close(nPipeOUT[0]);
            close(fd);
            free(args);
            wait(NULL);
        }
    }else perror("supervisor: ");
    
}


void manInput(int cmdPipe){
    char buf[PIPE_BUF];
    char c;
    int i=0;

    while(read(0,&c,1)>0){
        if(c=='\n'){
            buf[i++]=c;
            if(buf[0]==';') write(cmdPipe,buf,i);
            else write(1,buf,i);
            i=0;
        }else buf[i++]=c; 
    }   
}


void manOutput(void){

    int *pipes = (int*)calloc(INITS, sizeof(int)), i, nOut=0, defIn = open("/dev/null", O_RDONLY), idT, size=INITS;
    ssize_t r;
    char buf[PIPEBUF], pipeName[10];


    while(1){
        for(i = 0; read(0, buf+i, 1) > 0 && *(buf+i) != '\n' && i < PIPEBUF; i ++);
        if(buf[0] == ';'){ //check if it is a command
            if(i > 2){
                buf[i] = 0;
                idT = atoi(buf+2);
                switch(buf[1]){
                    case 'c':
                        getPipeName(buf+2, pipeName);
                        if((pipes[idT] = open(pipeName, O_WRONLY)) < 0) pipes[idT] = 0;
                    case 'd':
                        close(pipes[idT]);
                        pipes[idT] = 0;
                        break;
                    default:
                        break;
                }  

            }else fprintf(stderr, "supervisor: manOutput: Invalid command\n"); 
        }else{ 
            if(!nOut) write(defIn, buf, i); //output to /dev/null if no input is specified
            else 
                for(int j = 0; j < INITS; j ++)
                    if(pipes[j]) write(pipes[j], buf, i);
        }    
    }
}


void getPipeName(char *id, char *buf){
	buf[0]='n';
	buf[1]='o';
	buf[2]='d';
	buf[3]='e';
	strcat(buf,id);
}
