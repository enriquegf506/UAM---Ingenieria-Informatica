#include "vertex.h"
#include <string.h>

struct _Vertex {
  long id;
  char tag[TAG_LENGTH];
  Label state;
};

/*----------------------------------------------------------------------------------------*/
/*
Private function:
*/
Status vertex_setField (Vertex *v, char *key, char *value);

Status vertex_setField (Vertex *v, char *key, char *value) {
  if (!key || !value) return ERROR;

  if (strcmp(key, "id") == 0) {
    return vertex_setId(v, atol(value));
  } else if (strcmp(key, "tag") == 0) {
    return vertex_setTag(v, value);
  } else if (strcmp(key, "state") == 0) {
    return vertex_setState(v, (Label)atoi(value));
  }

  return ERROR;
}

/*----------------------------------------------------------------------------------------*/
Vertex *vertex_initFromString(char *descr){
  char buffer[1024];
  char *token;
  char *key;
  char *value;
  char *p;
  Vertex *v;

  /* Check args: */
  if (!descr) return NULL;

  /* Allocate memory for vertex: */
  v = vertex_init();
  if (!v) return NULL;

  /* Read and tokenize description: */
  sprintf(buffer, "%s", descr);
  token = strtok(buffer, " \t\n");
  while (token) {
    p = strchr(token, ':');
    if (!p) continue;

    *p = '\0';
    key = token;
    value = p+1;

    vertex_setField(v, key, value);

    token = strtok(NULL, " \t\n");
  }

  return v;
}

/**  rest of the functions in vertex.h **/


Vertex *vertex_init(){

  Vertex *vertex = NULL;

  vertex = (Vertex*)malloc(sizeof(Vertex));
  if(!vertex) return NULL;

  vertex->id = 0;
  vertex->state =  WHITE;
  strcpy(vertex->tag,"");

  if(!vertex->tag)
  {
    free(vertex);
    return NULL;
  }

  return vertex;
}

void vertex_free (void * v){
  
  if(!v) return;

  free(v);
}

long vertex_getId (const Vertex * v){
  long i_d = 0;

  if(!v) return -1;

  i_d = v->id;

  return i_d;
}

const char* vertex_getTag (const Vertex * v){

  if(!v) return NULL;

  return v->tag;
}

Label vertex_getState (const Vertex * v){
  if(!v) return ERROR_VERTEX;

  return v->state;
}

Status vertex_setId (Vertex * v, const long id){
  if(!v || id < 0) return ERROR;

  v->id = id;

  return OK;
}

Status vertex_setTag (Vertex * v, const char * tag){
  if(!v || !tag) return ERROR;

  strcpy(v->tag,tag);

  if(!v->tag) return ERROR;

  return OK;
}

Status vertex_setState (Vertex * v, const Label state){
  if(!v || state==ERROR_VERTEX) return ERROR;

  v->state = state;

  return OK;
}

int vertex_cmp (const void * v1, const void * v2){

  Vertex *v_1;
  Vertex *v_2;

  if(!v1 || !v2) return 0;

  v_1 = (Vertex*)v1;
  if(!v_1) return 0;
  v_2 = (Vertex*)v2;
  if(!v_2) return 0;

  if(v_1->id > v_2->id) return 1;
  else if(v_1->id < v_2->id) return -1;
  else{
    return(strcmp(v_1->tag,v_2->tag));
  }

  return 0;
}  

int vertex_print (FILE * pf, const void * v){
  Vertex *vertex;
  if(!pf || !v){
    return -1;
  }
  vertex = (Vertex*)v;
  if(vertex->state == WHITE){
    return fprintf(pf,"[%ld, %s, WHITE]", vertex->id, vertex->tag);
  }
  else if(vertex->state == BLACK){
    return fprintf(pf,"[%ld, %s, BLACK]", vertex->id, vertex->tag);
  }
  else{
    return fprintf(pf,"[%ld, %s, ERROR_VERTEX]", vertex->id, vertex->tag);
  }
}

void * vertex_copy (const void * src){
  Vertex *v, *vertex;
  if (!src){
    return NULL;
  }
  
  v = (Vertex*)malloc(sizeof(Vertex));
  if(v == NULL){
    return NULL;
  }
  
  
  vertex = (Vertex*)src;
  v->id = vertex->id;
  v->state = vertex->state;
  strcpy(v->tag,vertex->tag);
  if(!v->tag){
    vertex_free(v);
    return NULL;
  }
  return v;
}