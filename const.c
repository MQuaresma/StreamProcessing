#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void const (int x){
	char n[10];
	sprintf(n,"%d",x);
	char c;
	int fd1,fd2;
	
	fd1 = open("file",O_RDONLY);
	fd2 = open("aux",O_WRONLY | O_CREAT, 0664);
	if(fd1>0 && fd2>0){
		while(read(fd1,&c,1)>0){
			if(c=='\n'){
				write(fd2,":",1);
				write(fd2,n,strlen(n));
				write(fd2,"\n",1);
			}else write(fd2,&c,1);
		}
		close(fd1);
		close(fd2);
		rename("aux","file");
	}	
}
