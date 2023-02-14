#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

#define MAX_CHAR 128

int main(int argc, char *argv[])
{
    Graph *g = graph_init();
    FILE *fin = NULL;

    if(argc<2) return -1;
    

    fin = fopen(argv[1], "r");
    if(!fin) return -1;
    

    if(graph_readFromFile(fin, g) == OK){
        graph_print(stdout,g);
    }
    else return -1;

    graph_free(g);
    fclose(fin);

    return 0;
}
