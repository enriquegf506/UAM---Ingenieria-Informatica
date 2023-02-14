#include <stdio.h>
#include "vertex.h"


int main()
{
    Vertex *v1 = NULL;
    Vertex *v2 = NULL;
    Vertex *v3 = NULL;

    v2=vertex_init();
    if(!v2) return ERROR;

    v1=vertex_init();
    if(!v1) return ERROR;

    if(vertex_setId(v1,10) == ERROR){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }
    if(vertex_setTag(v1,"one") == ERROR){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }
    if(vertex_setState(v1,WHITE) == ERROR){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }

    if(vertex_setId(v2,20) == ERROR){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }
    if(vertex_setTag(v2,"two") == ERROR){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }
    if(vertex_setState(v2,BLACK) == ERROR){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }

    if(vertex_print(stdout,v1) == -1){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }
    if(vertex_print(stdout,v2) == -1){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }
    fprintf(stdout,"\n");

    if(vertex_cmp(v1,v2) != 0) fprintf(stdout,"Equals? No\n");
    else fprintf(stdout,"Equals? Yes\n");

    if(fprintf(stdout,"Vertex 2 tag: %s\n",vertex_getTag(v2)) <= 0){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }

    v3 = vertex_copy(v1);
    if(!v3){
        vertex_free(v1);
        vertex_free(v2);
        return ERROR;
    }

     if(fprintf(stdout,"Vertex 3 id: %ld\n",vertex_getId(v3)) <= 0){
        vertex_free(v1);
        vertex_free(v2);
        vertex_free(v3);
        return ERROR;
    }

    if(vertex_print(stdout,v1) == -1){
        vertex_free(v1);
        vertex_free(v2);
        vertex_free(v3);
        return ERROR;
    }
    if(vertex_print(stdout,v3) == -1){
        vertex_free(v1);
        vertex_free(v2);
        vertex_free(v3);
        return ERROR;
    }
    fprintf(stdout,"\n");

    if(vertex_cmp(v1,v3)!=0) fprintf(stdout,"Equals? No\n");
    else fprintf(stdout,"Equals? Yes\n");

    vertex_free(v3);
    vertex_free(v2);
    vertex_free(v1);

    return 0;
}