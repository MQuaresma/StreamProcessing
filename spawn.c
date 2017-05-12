#include <stdio.h>
#include <unistd.h>
#include <string.h>
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
	int eV=1;

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
								pos[v++]=i-1;
						}
				}
		}

		char c;
		char aux[10]; 
		int y=0, w=0, nCol, exitValue;
		char ret[PIPE_BUF];

		i=0;
		nCol=1;
		while(read(0,&c,1)>0){
			if(c=='\n'){
				if(v && y!=0){
					aux[y]='\0';
                   	args[pos[w++]]=strdup(aux);
                   	y=0;
				}
				exitValue=mySystem(args);
				sprintf(aux,"%d",exitValue);
				ret[i++]=':';
				for(w=0;w<strlen(aux); w++) ret[i++]=aux[w];
				ret[i++]='\n';
				write(1,ret,i);
				i=w=0;
				nCol=1;
			}else if(c==':') {
				if(v && y!=0){
					aux[y]='\0';
					args[pos[w++]]=strdup(aux);
					y=0;
				}
				ret[i++]=c;
				nCol++;
			}else{
				if(v && elem(col,v,nCol)) aux[y++]=c;
				ret[i++]=c;
			}
		}
		eV=0;

		for(i=0;i<argc-1; i++) free(args[i]);
		free(args);
	}
	return eV;
}
