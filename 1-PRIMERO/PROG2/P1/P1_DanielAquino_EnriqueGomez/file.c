#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#define MAX_CHAR 128

int main()
{
    FILE *f=NULL;
    int ids = 0;
    Graph *g=NULL;

    g = graph_init();

    f = fopen("file.txt","r");

    fscanf(f,"%d",&ids);
    fprintf(stdout,"%d\n",ids);


    fclose(f);
    graph_free(g);

    return 0;
}