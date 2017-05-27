#include "iStormAPI.h"

int main(int argc, char* argv[]) {
	char n[10];
	char c;
	char line[PIPE_BUF];
	int a=0;
	
    while(read(0,&c,1)>0) {
		if(c!='\n') {
			line[a]=c;
			a++;
		}else {
			line[a]='\n';
			a++;
			char * aux = (char*)malloc(sizeof(char)*a);
			strcpy(aux,line);
			int i = 0;
			int valor; 
			const char s[2]= ":";
			char * token;
			token = strtok(aux,s);

			int coluna = (atoi(argv[1])-1);
			int seccol = (atoi(argv[3])-1);
			int number,secnumber;
			while ((token!=NULL)&&(i<seccol)) {
				if (i==coluna) number = atoi(token);
				token = strtok(NULL,":");
                i++;
			}
			secnumber = atoi(token);

			if ((strcmp(argv[2],"=")==0) &&(number==secnumber)) write(1,line,a);
			if ((strcmp(argv[2],">=")==0)&&(number>=secnumber)) write(1,line,a);
			if ((strcmp(argv[2],"<=")==0)&&(number<=secnumber)) write(1,line,a);
			if ((strcmp(argv[2],">")==0) &&(number >secnumber)) write(1,line,a);
			if ((strcmp(argv[2],"<")==0) &&(number <secnumber)) write(1,line,a);
			if ((strcmp(argv[2],"!=")==0)&&(number!=secnumber)) write(1,line,a);
			a=0;
		}
	}
}
