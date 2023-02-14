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
#include "space.h"

/**
 * @brief Space
 *
 * This struct stores all the information of a space.
 */
struct _Space {
  Id id;                    /*!< Id number of the space, it must be unique */
  char name[WORD_SIZE + 1]; /*!< Name of the space */
  Set *objs;          /*!< Whether the space has an object or not */
  char gdesc[5][10];   /*!< graphic desctription*/
  char spacedescr[WORD_SIZE]; /*!< Description for the object */
};

/** space_create allocates memory for a new space
  *  and initializes its members
  */
Space* space_create(Id id) {
  Space *newSpace = NULL;
  int i,j;
  /* Error control */
  if (id == NO_ID)
    return NULL;

  newSpace = (Space *) malloc(sizeof (Space));
  if (newSpace == NULL) {
    return NULL;
  }

  /* Initialization of an empty space*/
  newSpace->id = id;
  newSpace->name[0] = '\0';
  newSpace->objs = set_create(1);
  for(j=0; j<5;j++){
    for (i = 0; i < 9; i++)
    {
      newSpace->gdesc[j][i] = '\0';
    }
  }
  return newSpace;
}

/** space_destroy frees the previous memory allocation 
  *  for a space
  */
STATUS space_destroy(Space* space) {
  if (!space) {
    return ERROR;
  }
  free(space->objs);
  free(space);
  return OK;
}

/** It gets the id of a space
  */
Id space_get_id(Space* space) {
  if (!space) {
    return NO_ID;
  }
  return space->id;
}

/** It sets the name of a space
  */
STATUS space_set_name(Space* space, char* name) {
  if (!space || !name) {
    return ERROR;
  }

  if (!strcpy(space->name, name)) {
    return ERROR;
  }
  return OK;
}
/** It gets the name of a space
  */
const char * space_get_name(Space* space) {
  if (!space) {
    return NULL;
  }
  return space->name;
}

/** It sets whether the space has an object or not
  */
STATUS space_add_object(Space* space, Id id) {
  if (!space) {
    return ERROR;
  }
  set_add(space->objs, id);
  return OK;
}
/** t deletes the object from the space
  */
STATUS space_del_object(Space* space, Id id) {
  if (!space) {
    return ERROR;
  }
  set_delete(space->objs, id);
  return OK;
}
/** It gets whether the space has an object or not
  */
Set *space_get_objects(Space* space) {
  if (!space) {
    return NULL;
  }
  return space->objs;
}
/** It determines whether the space has an object or not
  */
BOOL space_contain_object(Space* space, Id id)
{
  BOOL st=FALSE;
  if (!space)
    return (FALSE);
  st = set_findId(space->objs, id);
  return st;
}
/** It sets the space graphic description
  */
STATUS space_set_gdesc(Space* space, char *new, int posicion)
{
  if (!space || !new || posicion < 0 || posicion > 4)
    return ERROR;

  strcpy(space->gdesc[posicion], new);
  return OK;
}
/** It gets the space graphic description
  */
const char* space_get_gdesc(Space* space, int posicion)
{
  if (!space || posicion < 0 || posicion >4)
    return ERROR;
  return space->gdesc[posicion];
}

/** It sets the space description
  */
STATUS space_set_description(Space* space, char *description)
{
  if (!space || !description )
    return ERROR;

  strcpy(space->spacedescr, description);
  return OK;
}
/** It gets the space description
  */
char *space_get_description(Space *space)
{
  if (!space)
    return NULL;
  return space->spacedescr;
}

/** It prints the space information
  */
/*STATUS space_print(Space* space) {
  Id idaux = NO_ID;*/

  /* Error Control */
  /*if (!space) {
    return ERROR;
  }
*/
  /* 1. Print the id and the name of the space */
/*  fprintf(stdout, "--> Space (Id: %ld; Name: %s)\n", space->id, space->name);
*/
 
  /* 2. For each direction, print its link */ 
/*  idaux = space_get_north(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> North link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No north link.\n");
  }
  idaux = space_get_south(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> South link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No south link.\n");
  }
  idaux = space_get_east(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> East link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No east link.\n");
  }
  idaux = space_get_west(space);
  if (NO_ID != idaux) {
    fprintf(stdout, "---> West link: %ld.\n", idaux);
  } else {
    fprintf(stdout, "---> No west link.\n");
  }
*/
  /* 3. Print if there is an object in the space or not */
/*  if (space_get_object(space)) {
    fprintf(stdout, "---> Object in the space.\n");
  } else {
    fprintf(stdout, "---> No object in the space.\n");
  }

  return OK;
}
*/
