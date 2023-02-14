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
  char description[WORD_SIZE];   /* object description*/
  BOOL movable;   /*weather the object can be moved or not*/
  Id dependency;   /*whehter the object needs another object to be caught*/
  Id open;    /*Id of the link that a object can open*/
  BOOL illuminate;    /* TRUE if the object can illuminate an space*/
  BOOL turnedon;   /* TRUE if an object which can illuminate is turned on*/
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
STATUS object_set_name(Object* object, char* name) {
  if (!object || !name) {
    return ERROR;
  }

  if (!strcpy(object->name, name)) {
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

/** It sets the object description
  */
STATUS object_set_description(Object* object, char* description) {
  if (!object || !description)
    return ERROR;

  if (!strcpy(object->description, description))
    return ERROR;

  return OK;
}

/** It sets the object description
  */
char *object_get_description(Object *object)
{
  if (!object)
    return NULL;
  return object->description;
}

/** It gets the id dependency of an object
  */
Id object_get_dependency(Object* object) {
  if (!object) {
    return NO_ID;
  }
  return object->dependency;
}

/** It sets the id dependency of an object
  */
STATUS object_set_dependency(Object *object, Id id){
  if (object == NULL) {
    return ERROR;
  }

  object->dependency = id;
  return OK;
}

/** It gets the id of the link it can open
  */
Id object_get_open(Object* object) {
  if (!object) {
    return NO_ID;
  }
  return object->open;
}

/** It gets the id of the link it can open
  */
STATUS object_set_open(Object *object, Id id){
  if (object == NULL) {
    return ERROR;
  }

  object->open = id;
  return OK;
}

/** It gets the bool if the object can be moved
  */
BOOL object_get_movable(Object* object) {
  if (!object) {
    return -1;
  }
  return object->movable;
}

/** It gets the bool if the object can be moved
  */
STATUS object_set_movable(Object *object, BOOL mov){
  if (object == NULL || mov<0 || mov>1) {
    return ERROR;
  }

  object->movable = mov;
  return OK;
}

/** It gets the bool if the object can illuminate
  */
BOOL object_get_illuminate(Object* object) {
  if (!object) {
    return -1;
  }
  return object->illuminate;
}

/** It gets the bool if the object can illuminate
  */
STATUS object_set_illuminate(Object *object, BOOL ilu){
  if (object == NULL || ilu<0 || ilu >1) {
    return ERROR;
  }

  object->illuminate = ilu;
  return OK;
}

/** It gets the bool if the object is turned on
  */
BOOL object_get_turnedon(Object* object) {
  if (!object) {
    return -1;
  }
  return object->turnedon;
}

/** It gets the bool if the object is turned on
  */
STATUS object_set_turnedon(Object *object, BOOL turned){
  if (object == NULL || turned<0 || turned>1) {
    return ERROR;
  }

  object->turnedon = turned;
  return OK;
}


