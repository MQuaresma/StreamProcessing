#include <sys/wait.h>
#include<fcntl.h>
#include<signal.h>
#include "iStormAPI.h"
#define INITS 20

/*
 *
 */
int main(int argc, char *argv[]){

    int fd = open(argv[1], O_RDONLY); //open assigned pipe for communication with the controler
    int nPipeIN[2], nPipeOUT[2];
    char **args=NULL, *execN=NULL;
    pid_t proc=0, inC=0, outC=0;

    if(fd > 0){
        pipe(nPipeIN);
        pipe(nPipeOUT);
        
        args = (char**)calloc(argc-1, sizeof(void*));
        for(int i = 0; i < argc-1; args[i] = argv[i+2], i ++); //prepare arguments to be passed to execvp

        if(!strcmp(*args,"const") || !strcmp(*args,"window") || !strcmp(*args,"filter") || !strcmp(*args,"spawn")){
            execN = (char*)calloc(2+strlen(argv[2]), sizeof(char));
            execN[0] = '.';
            execN[1] = '/';
        }else execN = (char*)calloc(strlen(argv[2]), sizeof(char));
        strcat(execN, *args);

        if(!(proc = fork())){
            dup2(nPipeIN[0], 0);
            dup2(nPipeOUT[1], 1);
            close(nPipeIN[1]);
            close(nPipeIN[0]);
            close(nPipeOUT[1]);
            close(nPipeOUT[0]);
            close(fd);
            execvp(execN, args);
            perror("supervisor: execvp: ");
            _exit(1);
        }else{
            if(!(inC=fork())){ 
                dup2(fd, 0);     
                dup2(nPipeIN[1], 1);
                close(nPipeIN[1]);
                close(nPipeIN[0]);
                close(nPipeOUT[0]);
                close(fd);
                manInput(nPipeOUT[1]);
            }
            if(!(outC=fork())){
                dup2(nPipeOUT[0], 0); 
                close(nPipeIN[1]);
                close(nPipeIN[0]);
                close(nPipeOUT[1]);
                close(nPipeOUT[0]);
                close(fd);
                manOutput(inC);
            }
            close(nPipeIN[0]);
            close(nPipeIN[1]);
            close(nPipeOUT[1]);
            close(nPipeOUT[0]);
            close(fd);
        }
    }else perror("supervisor: ");
    return 0;    
}


void manInput(int cmdPipe){
    char buf[PIPE_BUF];
    char c;
    int i=0;

    while(1){
        while(read(0,&c,1)>0){
            buf[i++]=c;
            if(c=='\n'){
                if(buf[0]==';') write(cmdPipe,buf,i);
                else write(1,buf,i);
                i=0;
                kill(getpid(),SIGSTOP);
            }
        }   
    }    
}


void manOutput(pid_t manIn){
    int *pipes = (int*)calloc(INITS, sizeof(int)), i, nOut=0, defIn = open("/dev/null", O_WRONLY | O_APPEND,0777), idT, size=INITS;
    char buf[PIPE_BUF], pipeName[10];

    while(1){
        for(i = 0; read(0, buf+i, 1) > 0 && *(buf+i) != '\n' && i < PIPE_BUF; i ++);
        if(buf[0] == ';'){ //check if it is a command
            if(i > 2){
                buf[i] = 0;
                idT = atoi(buf+2);
                switch(buf[1]){
                    case 'c':
                        getPipeName(buf+2, pipeName);
                        size *= 1.5;
                        if(idT >= size) pipes=(int*)realloc(pipes, sizeof(int)*size);
                        if((pipes[idT] = open(pipeName, O_WRONLY)) < 0) pipes[idT] = 0;
                        else nOut++;
                        break;
                    case 'd':
                        close(pipes[idT]);
                        pipes[idT] = 0;
                        nOut--;
                        break;
                    default:
                        break;
                }  
            }else fprintf(stderr, "supervisor: manOutput: Invalid command\n"); 
            buf[0]=0;
        }else{ 
            if(nOut<=0)
                write(defIn, buf, i+1); //output to /dev/null if no input is specified
            else 
                for(int j = 0; j < size; j ++)
                    if(pipes[j]) write(pipes[j], buf, i+1);
        }
        kill(manIn,SIGCONT);
    }
}


void getPipeName(char *id, char *buf){
	buf[0]='n';
	buf[1]='o';
	buf[2]='d';
	buf[3]='e';
    buf[4]=0;
	strcat(buf,id);
}
