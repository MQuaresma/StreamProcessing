#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main (int argc, char *argv[]){
	int ret=1;

	if(argc==2){
            char c;
            int fd1,fd2;
   
            fd1 = open("file",O_RDONLY);
            if(fd1>0){
                    while(read(fd1,&c,1)>0){
                            if(c=='\n'){
                                    write(1,":",1);
                                    write(1,argv[1],strlen(argv[1]));
                                    write(1,"\n",1);
                            }else write(1,&c,1);
                    }
                    close(fd1);
                    close(fd2);
                    ret=0;
            }
	}
	return ret;	
}
