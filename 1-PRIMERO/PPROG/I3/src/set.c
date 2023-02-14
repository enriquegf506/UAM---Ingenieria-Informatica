/**
 * @brief It defines the set interface
 *
 * @file set.c
 * @author Enrique Gómez Fernández 
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

/**
 * @brief Set
 *
 * This struct stores all the information of a set.
 */
struct _Set{
    
    Id ids[MAX_SETIDS]; /*!< Id array of ids */ 
    int n_ids;          /*!< Number of ids we have on the array*/ 
};
/** set_create allocates memory for a new object
  *  and initializes its members
  */
Set* set_create(Id id){
  Set *newSet = NULL;
  int i;
  /* Error control */
  if (id == NO_ID)
    return NULL;

  newSet = (Set *) malloc(sizeof (Set));
  if (newSet == NULL) {
    return NULL;
  }

  /* Initialization of an empty space*/
  for(i=0; i< MAX_SETIDS; i++){
    newSet->ids[i]= NO_ID;
  }
  
  newSet->n_ids=0;

  return newSet;
}


/** set_destroy frees the previous allocated memory for a set
  */
STATUS set_destroy(Set* set){
  if (!set) {
    return ERROR;
  }

  free(set);
  set = NULL;
  return OK;
}

/**
 It gets the ids 
*/
Id *set_getIds(Set *set){
  if(!set){
    return NULL;
  }
    return set->ids;
}


/**
 It gets the number of ids of a set
*/
int set_getNids(Set* set){
  if (!set) {
    return -1;
  }
  return set->n_ids;
}

/**
 It finds an Id the set
 */
BOOL set_findId(Set* set, Id id){
  int i = 0;

  if( set == NULL)
    return FALSE;

  for (i = 0; i < 5; i++)
  {
    if(set->ids[i] == id)
      return TRUE;
  }
  
  return FALSE;
}

/**
 It adds an Id the set
 */
STATUS set_add(Set* set, Id id){
  int i=0;
  if (!set || !id){
    return ERROR;
  }
  while (set->ids[i] != NO_ID)
  {
    i++;
  }
/* Error handling*/
  if (i >= MAX_SETIDS)
  {
    return ERROR;
  }
/*Creation of the new id*/

  set->ids[i] = id;
  set->n_ids++;
  return OK;
}
/**
 It delete the set
 */
STATUS set_delete(Set* set, Id id){
  int i;
  
  if (!set || id == NO_ID )
    return ERROR;
    
  for (i=0; i< set->n_ids; i++){
    if(set->ids[i] == id){
      set->ids[i] = NO_ID;
    }
  }
  set->n_ids--;
  return OK;
}

/**
 It prints the set
 */
STATUS set_print(FILE *f,Set *set){
    int i;
  if(set == NULL|| f==NULL)
    return ERROR;
  
  for ( i = 0; i <set->n_ids; i++)
  {
     fprintf(f,"\nObjeto %ld", set->ids[i]);
      
  }
  return OK;
  
}
