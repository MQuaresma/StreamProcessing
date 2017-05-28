#include <stdio.h>
#include "iStormAPI.h"

void getValues(int op, int line, int col);
int getMin(int *buf, int v);
int getMax(int *buf, int v);
int getSum(int *buf, int v);

int main(int argc, char *argv[]){
    int col, lines, op;

    if(--argc < 3){
        fprintf(stderr, "%s: check manual for usage\n", argv[0]);
        _exit(1);
    }

    sscanf(argv[1], "%d", &col);
    sscanf(argv[3], "%d", &lines);

    // fun with bitwise level operators
    op = (argv[2][1]=='v') ^ (argv[2][1]=='i')<<1 ^ (argv[2][1]=='a')<<2 ^ (argv[2][1]=='u')<<3;
   
    getValues(op, lines, col);
    return 0;
}


void getValues(int op, int line, int col){
    int temp, res = 0, *valBuf = (int*)calloc(line, sizeof(int));
    char buf, out[PIPE_BUF+10];

    for(int curCol = 1, i = 0, v = 0, j = 0; valBuf && read(0, &buf, 1) > 0; curCol += (buf == ':')){
        if(curCol == col){
            temp = 0;
            do{
                temp = temp*10+(buf-'0');
                out[j++] = buf;
            }while(read(0, &buf, 1) > 0 && buf != ':' && buf != '\n');
        }
        if(buf == '\n'){
            curCol = 1;
            sprintf(out+j, ":%d\n", res);
            write(1, out, strlen(out));
            j=0; 
            if(i == line) i = 0;
            valBuf[i++] = temp;
            if(v < line) v ++;
            switch(op){
                case 1:
                    res = getSum(valBuf, v);
                    if(v > 0) res /= v;
                    break;
                case 2:
                    res = getMin(valBuf, v);
                    break;
                case 4:
                    res = getMax(valBuf, v);
                    break;
                case 8:
                    res = getSum(valBuf, v);
                    break;
            }
        }else out[j++] = buf; 
    }
    if(valBuf) free(valBuf);
}


int getSum(int *buf, int v){
    int res=0;

    for(int i = 0; i < v; res += buf[i], i ++);

    return res;
}

int getMin(int *buf, int v){
    int res = INT_MAX;
    
    for(int i = 0; i < v; i++)
        if(res > buf[i]) res = buf[i];

    return (res == INT_MAX ? 0 : res);
}

int getMax(int *buf, int v){
    int res = INT_MIN;
    
    for(int i = 0; i < v; i++)
        if(res < buf[i]) res = buf[i];

    return (res == INT_MIN ? 0 : res);
}
