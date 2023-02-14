#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "point.h"
#include "map.h"
#include "stack_fDoble.h"

#define MAX_TAM 1024

int main(int argc, char **argv) {
    char file[MAX_TAM] = "\0";
    FILE *f = NULL;
    Map *mp = NULL;

    if(argc < 2) {
        fprintf(stdout, "Faltan argumentos: ./exe file\n");
        return -1;
    }

    strcpy(file, argv[1]);

    f = fopen(file, "r");
    if(!f){
        return 0;
    }

    mp = map_readFromFile(f);
    if(!mp){
        return 0;
    }


    fprintf(stdout, "Maze: \n");
    if(map_print(stdout, mp) == -1){
        return 0;
    }

    fprintf(stdout, "DFS traverse: \n");
    if(map_dfs(stdout, mp) == NULL){
        return 0;
    }

    map_free(mp);
    fclose(f);

}
