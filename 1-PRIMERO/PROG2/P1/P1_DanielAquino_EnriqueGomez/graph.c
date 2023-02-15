#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "vertex.h"

#define MAX_VTX 4096
#define MAX_LINE 128

struct _Graph
{
Vertex *vertices[MAX_VTX];
Bool connections[MAX_VTX][MAX_VTX];
int num_vertices;
int num_edges;
};

/**
 * @brief Creates a new empty graph.
 *
 * Allocates memory for a new graph and initializes it to be empty 
 * (no vertices and no edges).
 *
 * @return A pointer to the graph if it was correctly allocated, 
 * NULL otherwise.
 **/
Graph * graph_init(){
    int i=0,j=0;
    Graph *graph = NULL;

    graph = (Graph *)malloc(sizeof(Graph));
    if (!graph) return NULL;


    
    for(i=0; i<MAX_VTX;i++){
        graph->vertices[i] = NULL;
        for(j=0; j<MAX_VTX; j++){
            graph->connections[i][j]=FALSE;
        }
    }
    graph->num_edges=0;
    graph->num_vertices=0;
    return graph;
}

/**
 * @brief Frees a graph.
 *
 * Frees all the memory allocated for the graph.
 *
 * @param g Pointer to graph to be freed.
 **/
void graph_free(Graph *g){
    int i=0;

    if(!g){
        return;
    }

    for(i=0; i<g->num_vertices; i++){
        vertex_free(g->vertices[i]);
    }
    free(g);

}

/**
 * @brief Inserts a new vertex in a graph.
 *
 * Creates a vertex by calling vertex_initFromString and adds it to
 * a graph. If a vertex with the same id already exists in the graph, 
 * it is not added. 
 *
 * @param g Pointer to the graph.
 * @param desc Description of the vertex.
 *
 * @return Returns OK if the vertex could be created (or if it exists 
 * already), ERROR otherwise.
 **/
Status graph_newVertex(Graph *g, char *desc){

    Vertex *v=NULL;
    int i=0;

    if(!g || !desc) return ERROR;

    v = vertex_initFromString(desc);

    if(!v) return ERROR;

    for(i=0;i<g->num_vertices;i++)
    {
        if(vertex_cmp(g->vertices[i],v)==0){
            vertex_free(v);
            return OK;
        }
    }

    g->vertices[g->num_vertices]=v;
    g->num_vertices+=1;

    return OK;
}

/**
 * @brief Creates an edge between to vertices of a graph.
 *
 * If any of the two vertices does not exist in the graph the edge is
 * not created.
 *
 * @param g Pointer to the graph.
 * @param orig ID of the origin vertex.
 * @param dest ID of the destination vertex.
 *
 * @return OK if the edge could be added to the graph, ERROR otherwise.
 **/
Status graph_newEdge(Graph *g, long orig, long dest)
{
    int i=0,flag1=0,flag2=0, id=0;

    if(!g || orig<0 || dest<0) return ERROR;

    flag1=-1;
    flag2=-1;
    for(i=0;i<g->num_vertices;i++)
    {
        id=vertex_getId(g->vertices[i]);
        if(id==orig) flag1=i;
        else if(id==dest) flag2=i;
    }

    if(flag1>=0 && flag2>=0){
        g->connections[flag1][flag2]=TRUE;
        g->num_edges++;
    }
    else return ERROR;

    return OK;
}

/**
 * @brief Checks if a graph contains a vertex.
 *
 * @param g Pointer to the graph.
 * @param id ID of the vertex.
 *
 * @return Returns TRUE if there is a vertex in the graph g with the
 * ID id, FALSE otherwise.
 **/
Bool graph_contains(const Graph *g, long id){
    int i=0;

    if(!g || id<0){
        return FALSE;
    }

    for(i=0; i<graph_getNumberOfVertices(g); i++){
        if(vertex_getId(g->vertices[i])==id){
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @brief Returns the total number of vertices in a graph.
 *
 * @param g Pointer to the graph.
 *
 * @return Returns The number of vertices in the graph, or -1 if 
 * there is any error.
 **/
int graph_getNumberOfVertices(const Graph *g){

    if(!g){
        return -1;
    }
    return g->num_vertices;
}

/**
 * @brief Returns the total number of edges  * in the graph.
 *
 * @param g Pointer to the graph.
 *
 * @return Returns The number of vertices in the graph, or -1 if 
 * there is any error.
 **/
int graph_getNumberOfEdges(const Graph *g){
    
    if(!g){
        return -1;
    }
    return g->num_edges;
}

/**
 * @brief Determines if there is a connection between a pair of vertices.
 *
 * @param g Pointer to the graph.
 * @param orig ID of the origin vertex.
 * @param dest ID of the destination vertex.
 *
 * @return Returns TRUE if there is a connection in g from orig
 *  to dest, FALSE otherwise.
 **/
Bool graph_connectionExists(const Graph *g, long orig, long dest){
    int i=0, numvert=0, x=0, y=0;

    if(!g || orig<0 || dest<0 || graph_contains(g,orig)==FALSE || graph_contains(g,dest)==FALSE){
        return FALSE;
    }

    numvert= graph_getNumberOfVertices(g);
    for(i=0; i<numvert; i++){
        if(vertex_getId(g->vertices[i])==orig){
            x=i;
        }
        if(vertex_getId(g->vertices[i])==dest){
            y=i;
        }
    }

    return g->connections[x][y];
}

/**
 * @brief Gets the number of connections starting at a given vertex.
 *
 * @param g Pointer to the graph.
 * @param id ID of the origin vertex.
 *
 * @return Returns the total number of connections starting at 
 * vertex with ID id, or -1 if there is any error.
 **/
int graph_getNumberOfConnectionsFromId(const Graph *g, long id){
    int i=0, count=0, numvert=0, x=0;
    if(!g || id<0 || graph_contains(g,id)==FALSE){
        return -1;
    }

    numvert=graph_getNumberOfVertices(g);
    for(i=0;i<numvert; i++){
        if(vertex_getId(g->vertices[i])==id){
            x=i;
        }
    }
    for(i=0;i<numvert; i++){
        if(g->connections[x][i]==TRUE){
            count++;
        }
    }
    return count;
}

/**
 * @brief Returns an array with the ids of all the vertices which a 
 * given vertex connects to.
 *
 * This function allocates memory for the array.
 *
 * @param g Pointer to the graph.
 * @param id ID of the origin vertex.
 *
 * @return Returns an array with the ids of all the vertices to which 
 * the vertex with ID id is connected, or NULL if there is any error.
 */
long *graph_getConnectionsFromId(const Graph *g, long id){
    int i=0, j=0, numvert=0, x=0;
    long *array=NULL;

    if(!g || id<0 || graph_contains(g,id)==FALSE){
        return NULL;
    }

    array = (long*)malloc(sizeof(long)*MAX_VTX);

    if(!array) return NULL;

    numvert=graph_getNumberOfVertices(g);

    for(i=0;i<numvert; i++){
        if(vertex_getId(g->vertices[i])==id){
            x=i;
        }
    }
    for(i=0,j=0;i<numvert; i++){
        if(g->connections[x][i]==TRUE){
            array[j] = vertex_getId(g->vertices[i]);
            if(!array[j]) return NULL;
            j++;
        }
    }
    return array;
}

/**
 * @brief Gets the number of connections starting at a given vertex.
 *
 * @param g Pointer to the graph.
 * @param tag Tag of the origin vertex.
 *
 * @return Returns the total number of connections starting at 
 * vertex with Tag tag, or -1 if there is any error.
 **/
int graph_getNumberOfConnectionsFromTag(const Graph *g, char *tag){
    int i=0, numvert=0, x=-1, count=0;
    if(!g || !tag){
        return -1;
    }

    numvert=graph_getNumberOfVertices(g);
    for(i=0; i<numvert;i++){
        if(strcmp(vertex_getTag(g->vertices[i]), tag)==0){
            x=i;
        }
    }
    if(x<0 || x>=numvert){
        return -1;
    }

    for(i=0; i<numvert;i++){
        if(g->connections[x][i]==TRUE){
            count++;
        }
    }
    return count;
}

/**
 * @brief Returns an array with the ids of all the vertices which a 
 * given vertex connects to.
 *
 * This function allocates memory for the array.
 *
 * @param g Pointer to the graph.
 * @param tag Tag of the origin vertex.
 *
 * @return Returns an array with the ids of all the vertices to which 
 * the vertex with Tag tag is connected, or NULL if there is any error.
 */
long *graph_getConnectionsFromTag(const Graph *g, char *tag)
{
    long *array = NULL;
    int i=0,x=0,j=0,flag=0;

    if(!g || !tag){
        return NULL;
    }

    array = (long*)malloc(sizeof(long)*MAX_VTX);
    if(!array) return NULL;

    for(i=0;i<graph_getNumberOfVertices(g);i++)
    {
        if(strcmp(tag,vertex_getTag(g->vertices[i]))==0)
        {
            x=i;
            flag=0;
        }
        
    }
    if(flag==0)
    {
        for(i=0,j=0;i<MAX_VTX;i++)
        {
            if((g->connections[x][i])==TRUE)
            {
                array[j] = vertex_getId(g->vertices[i]);
                if(!array[j]) return NULL;
                j++;
            }
        }      
    }
    else return NULL;

    return array;
}


/**
 * @brief Prints a graph.
 *
 * Prints the graph g to the file pf.
 * The format to be followed is: print a line by vertex with the 
 * information associated with the vertex and the id of their connections:
 *
 * For example:
 * [1, Madrid, 0]: [2, Toledo, 0] [3, Avila, 0] 
 * [2, Toledo, 0]: [4, Segovia, 0] 
 * [3, Avila, 0]: 
 * [4, Segovia, 0]: [3, Avila, 0]
 *
 * @param pf File descriptor.
 * @param g Pointer to the graph.
 *
 * @return The number of characters printed, or -1 if there is any error.
 */
int graph_print (FILE *pf, const Graph *g){
    int i=0, numvert, j=0, count=0;
    if(!pf || !g){
        return -1;
    }

    numvert=graph_getNumberOfVertices(g);
    for(i=0;i<numvert; i++){
        count+=vertex_print(pf, g->vertices[i]);
        fprintf(pf,":");
        for(j=0;j<numvert;j++){
            if(g->connections[i][j]==TRUE){
                count+=vertex_print(pf, g->vertices[j]);
            }
        }
        fprintf(pf,"\n");
    }
    return count;
}

/**
 * @brief Reads a graph definition from a text file.
 *
 * Reads a graph description from the text file pointed to by fin,
 * and fills the graph g.
 *
 * The first line in the file contains the number of vertices.
 * Then one line per vertex with the vertex description.  
 * Finally one line per connection, with the ids of the origin and 
 * the destination. 
 *
 * For example:
 *
 * 4
 * id:1 tag:Madrid
 * id:2 tag:Toledo
 * id:3 tag:Avila
 * id:4 tag:Segovia
 * 1 2
 * 1 3
 * 2 4
 * 4 3
 *
 * @param fin Pointer to the input stream.
 * @param g Pointer to the graph.
 *
 * @return OK or ERROR
 */
Status graph_readFromFile (FILE *fin, Graph *g)
{
    char line[MAX_LINE];
    int num_vertices=0;
    int i=0;
    long orig=0, dest=0;
    int ret_value=0;

    if(!fin || !g) return ERROR;

    if(fgets(line, MAX_LINE, fin) == NULL){
        return ERROR;
    }

    ret_value = sscanf(line, "%d", &num_vertices);
    if(ret_value!=1){
        return ERROR;
    }


    for(i=0; i<num_vertices; i++){
        if(fgets(line, MAX_LINE, fin) == NULL){
            return ERROR;
        }
        ret_value = graph_newVertex(g, line);
        if(ret_value == ERROR){
            return ERROR;
        }
    }

    while(fgets(line, MAX_LINE, fin) != NULL){
        ret_value = sscanf(line, "%ld %ld", &orig, &dest);
        if(ret_value != 2){
            return ERROR;
        }
        ret_value = graph_newEdge(g, orig, dest);
        if(ret_value == ERROR){
            return ERROR;
        }
    }
    

    return OK;
}