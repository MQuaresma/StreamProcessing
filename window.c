#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void getValues(int op, int line, int col);
int getMin(int *buf, int v);
int getMax(int *buf, int v);
int getSum(int *buf, int v);

main(int argc, char *argv[]){

    int col, lines, op;

    if(--argc < 3){
        fprintf(stderr, "%s: check manual for usage\n", argv[0]);
        _exit(1);
    }

    sscanf(argv[1], "%d", &col);
    sscanf(argv[3], "%d", &lines);

    if(!strcmp(argv[2], "avg")) op = 1;
    else if(!strcmp(argv[2], "min")) op = 2;
    if(!strcmp(argv[2], "max")) op = 3;
    if(!strcmp(argv[2], "sum")) op = 4;

    /* Fun way of doing it
     * op = (argv[2][2]=='v')<<1 ^ (argv[2][2]=='i')<<2 ^ (argv[2][2]=='a')<<3 ^ (argv[2][2]=='u')<<4;
     */
   
    getValues(op, lines, col);
    
}


void getValues(int op, int line, int col){

    int temp, res = 0, *valBuf;
    char buf, resOut[10];

    valBuf = (int*)calloc(line, sizeof(int));

    for(int curCol = 1, i = 0, v = 0; read(0, &buf, 1) > 0; curCol += (buf == ':')){
        if(curCol == col){
            temp = 0;
            do{
                temp = temp*10+(buf-'0');
                write(1, &buf, 1);
            }while(read(0, &buf, 1) > 0 && buf != ':' && buf != '\n');
        }
        if(buf == '\n'){
            curCol = 1;
            sprintf(resOut, ":%d", res);
            write(1, resOut, strlen(resOut));
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
                case 3:
                    res = getMax(valBuf, v);
                    break;
                case 4:
                    res = getSum(valBuf, v);
                    break;
                defaut:
                    fprintf(stderr, "Option not available\n");
                    break;
            }
        }
        write(1, &buf, 1);
    }

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
