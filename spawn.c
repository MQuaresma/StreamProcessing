#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int mySystem(char *arg[]){
	int ret=1;
	
	if(fork()==0){
		execvp(*arg,arg);
		_exit(1);
	}else{
		wait(&ret);
		ret=WEXITSTATUS(ret);
	}
	return ret;
}

int elem (int *arr, int n, int x){
	int i, found=0;
	for(i=0; i<n && !found; i++)
		if(x==arr[i]) found=1;
	return found;
}

int main (int argc, char *argv[]){
	int ret=1;

	if(argc>1){
		int i, v;
		char **args = (char**)calloc(argc, sizeof(char));
		for(i=1, v=0; i<argc; i++){
			if((*argv[i])=='$') v++;
			args[i-1]=strdup(argv[i]);
		}
		
		int col[v], pos[v];
		if(v!=0){	
				for(i=1, v=0; i<argc; i++){
						if(*argv[i]=='$') {
								col[v]=atoi(argv[i]+1);
								pos[v]=i-1;
								v++;
						}
				}
		}

		char c;
		char n[2];
		char aux[10]; 
		int fd1, fd2, y=0, w=0, exitValue;
	
		fd1 = open("file",O_RDONLY);
		if(fd1>0){
			i=1;
			while(read(fd1,&c,1)>0){
				if(c=='\n'){
					exitValue=mySystem(args);
					sprintf(n,"%d",exitValue);
					write(1,":",1);
					write(1,n,strlen(n));
					write(1,"\n",1);
				}else if(c==':') {
					if(v && y!=0){
						aux[y]='\0';
						args[pos[w]]=strdup(aux);
						w++;
						y=0;
					}
					write(1,&c,1);
					i++;
				}else{
					if(v && elem(col,v,i)){
						aux[y]=c;
						y++;           
					}
					write(1,&c,1);
				}
			}
			close(fd1);
			close(fd2);
			ret=0;
		}
		for(i=0;i<argc-1; i++){
			free(args[i]);
		}
		free(args);
	}
	return ret;
}
