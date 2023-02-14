/**
 * @brief It defines the object interface
 *
 * @file object.c
 * @author Daniel Aquino Santiago
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"


struct _Object{
  Id id;
  char name[WORD_SIZE + 1];
};

Object* object_create(Id id){
  Object *newObject = NULL;

  if (id == NO_ID)
    return NULL;

  newObject = (Object *) malloc(sizeof (Object));
  if (newObject == NULL)
    return NULL;

  newObject->id = id;
  newObject->name[0] = '\0';
  return newObject;
}


STATUS object_destroy(Object* object){
  if (!object)
    return ERROR;

  free(object);
  object = NULL;
  return OK;
}


Id object_get_id(Object* object){
  if (!object)
    return NO_ID;

  return object->id;
}


STATUS object_set_id(Object* object, Id id){
  if(!object||id==NO_ID)
    return ERROR;

  object->id=id;
  return OK;
}


STATUS object_set_name(Object* object, char* name) {
  if (!object || !name)
    return ERROR;

  if (!strcpy(object->name, name))
    return ERROR;

  return OK;
}


const char * object_get_name(Object* object) {
  if (!object)
    return NULL;

  return object->name;
}


STATUS object_print(Object* object){
  if (!object)
    return ERROR;

  fprintf(stdout, "-->object (id: %ld; name: %s)\n", object->id, object->name);
  return OK;
}