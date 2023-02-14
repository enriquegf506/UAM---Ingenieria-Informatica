@@ -1,85 +0,0 @@
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

/**
 * @brief Object
 *
 * Estructura que contiene la información del Objeto
 */

struct _Object{
    Id id; /* Id del objeto */
    char name[WORD_SIZE + 1]; /* nombre del objeto */
    Id space; /* Id del espacio en el que está */
};

/** object_create guarda mamoria para un nuevo objeto
  *  e inicializa sus partes.
  */
  
Object* object_create(Id id) {
  Object *newObject = NULL;

  /* Control de errores */
  if (id == NO_ID)
    return NULL;

  newObject = (Object *) malloc(sizeof (Object));
  if (newObject == NULL) {
    return NULL;
  }
  
  /* Iniciar un objeto */
  
  newObject->id = id;
  newObject->name = "\0"; 
  newObject->space = NO_ID;
}

/** object_destroy libera la memoria 
  *  para un objeto
  */
  
 STATUS object_destroy(Object* object) {
  if (object == NULL) {
    return ERROR;
  }

  free(object);
  object = NULL;
  return OK;
}

/** Coge el Id del objeto
  */
  
Id object_get_id(Object* object) {
  if (object == NULL) {
    return NO_ID;
  }
  return object->id;
}
  
 /** Guarda el nombre del objeto.
  */
STATUS object_set_name(Object* object, char* name) {
  if (object == NULL || name == NULL) {
    return ERROR;
  }

  if (!strcpy(object->name, name)) {
    return ERROR;
  }
  return OK;
}

 /** Guarda el nombre del espacio en el que está el objeto
  */
Id * object_get_name(Object* object) {
  if (object == NULL) {
    return NULL;
  }
  return object->space;
}