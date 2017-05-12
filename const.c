#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]){
	int eV=1;

	if(argc==2){
    	char c;
        int i=0, w;
		char ret[PIPE_BUF];
   
        while(read(0,&c,1)>0){
			if(c=='\n'){
				ret[i++]=':';
				for(w=0;w<strlen(argv[1]);w++) ret[i++]=argv[1][w];
				ret[i++]='\n';
				write(1,ret,i);
				i=0;
			}else ret[i++]=c;
		}
		eV=0;
    }
	return eV;
}
