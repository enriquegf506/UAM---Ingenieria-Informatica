/** 
 * @brief It implements the space module
 * 
 * @file space.c
 * @author Enrique Gomez
 * @version 2.0 
 * @date 29-11-2021 
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

/**
 * @brief Object
 *
 * This struct stores all the information of an object.
 */
struct _Object {
  Id id;                    /* Id of the object */
  char name[WORD_SIZE + 1]; /* object name*/
};

/** object_create allocates memory for a new object
  *  and initializes its members
  */
Object* object_create(Id id) {
  Object *newObject = NULL;

  /* Error control */
  if (id == NO_ID)
    return NULL;

  newObject = (Object *) malloc(sizeof (Object));
  if (newObject == NULL) {
    return NULL;
  }

  /* Initialization of an empty object*/
  newObject->id = id;
  newObject->name[0] = '\0';

  return newObject;
}

/** object_destroy frees the previous memory allocation 
  *  for an object
  */
STATUS object_destroy(Object* object) {
  if (!object) {
    return ERROR;
  }

  free(object);
  object = NULL;
  return OK;
}

/** It gets the id of an object
  */
Id object_get_id(Object* object) {
  if (!object) {
    return NO_ID;
  }
  return object->id;
}

/** It sets the id of an object
  */
STATUS object_set_id(Object *object, Id id){
  if (object == NULL || id == NO_ID) {
    return ERROR;
  }

  object->id = id;
  return OK;
}

/** It sets the name of an object
  */
STATUS object_set_name(Object* space, char* name) {
  if (!space || !name) {
    return ERROR;
  }

  if (!strcpy(space->name, name)) {
    return ERROR;
  }
  return OK;
}

/** It gets the name of an object
  */
const char * object_get_name(Object* object) {
  if (!object) {
    return NULL;
  }
  return object->name;
}


/** It prints the object information
  */
STATUS object_print(Object* object) {
  if (!object) {
    return ERROR;
  }

  /* 1. Prints the Id y and the objet name */
  fprintf(stdout, "--> Object (Id: %ld; Name: %s)\n", object->id, object->name);
 
  return OK;
}
