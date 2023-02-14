#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graph.h"


int main()
    {
        Graph *g = NULL;
        int num = 0,i = 0;
        long *array = NULL;

        g = graph_init();
        if(!g) return -1;

        if(graph_newVertex(g,"id:111 tag:Madrid\n") == ERROR)
        {
            graph_free(g);
            return -1;
        }
        if(graph_newVertex(g,"id:222 tag:Toledo\n") == ERROR)
        {
            graph_free(g);
            return -1;
        }


        if(graph_contains(g,111) == TRUE) fprintf(stdout,"Inserting Madrid... result...:1\n");
        else fprintf(stdout,"Inserting Madrid... result...:0\n");

        if(graph_contains(g,222) == TRUE) fprintf(stdout,"Inserting Toledo... result...:1\n");
        else fprintf(stdout,"Inserting Toledo... result...:0\n");
        fprintf(stdout,"Inserting edge: 222 --> 111\n");
        if(graph_newEdge(g,222,111) == ERROR) return -1;

        if(graph_connectionExists(g,111,222) == TRUE) fprintf(stdout,"111 --> 222? yes\n");
        else  fprintf(stdout,"111 --> 222? No\n");

        if(graph_connectionExists(g,222,111) == TRUE) fprintf(stdout,"222 --> 111? yes\n");
        else  fprintf(stdout,"222 --> 111? No\n");

        num = graph_getNumberOfConnectionsFromId(g,111);

        if(num == -1)
        {
            graph_free(g);
            return -1;
        }

        fprintf(stdout,"Number of connections from 111: %d\n",num);


        num = graph_getNumberOfConnectionsFromTag(g,"Toledo");

        if(num == -1)
        {
            graph_free(g);
            return -1;
        }

        fprintf(stdout,"Number of connections from Toledo: %d\n",num);

        array = graph_getConnectionsFromTag(g,"Toledo");

        if(!array)
        {
            graph_free(g);
            return -1;
        }

        fprintf(stdout,"Connections from Toledo: ");
        i = 0;
        do
        {
            fprintf(stdout,"%ld",array[i]);
            i++;
        } while (i < num);

        fprintf(stdout,"\nGraph:\n");
        if(graph_print(stdout,g) == -1)
        {
            graph_free(g);
            return -1;
        }

        graph_free(g);
        free(array);

        return 0;
    }