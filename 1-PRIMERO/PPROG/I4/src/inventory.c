/**
 * @brief It implements the inventory module
 *
 * @file inventory.c
 * @author Enrique Gómez
 * @version 2.0
 * @date 27-02-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventory.h"

/**
 * @brief Inventory
 * This struct stores all the information of a inventory.
 */

struct _Inventory {

  Set *objects;      /*!< Set of the objects of the player */
  int max_objs;     /*!< Max Number of objects the player can port*/ 

};
/** inventory_create allocates memory for a new inventory
  *  and initializes its members
  */
Inventory* inventory_create(){
  Inventory *NewInventory;
  /*allocates memory for a new inventory*/ 
  NewInventory=(Inventory*) malloc(sizeof(Inventory));
  /*Error control*/
  if (NewInventory == NULL)
    return NULL;
  
  NewInventory->objects= set_create(1);
  NewInventory->max_objs= MAX_PORT;
  
  return NewInventory;
}
/** inventory_destroy frees the previous memory allocation 
  *  for an inventory
  */
STATUS inventory_destroy(Inventory* inv){
  if(!inv){
    return ERROR;
  }
  /*destroy the set of inventory*/
  if(set_destroy(inv->objects)==ERROR){
    return ERROR;
  }
  free(inv);
  return OK;
}
/**
 It sets the maximum number of ids of a inventory
*/
STATUS inventory_set_max(Inventory *inv, int max){
  if (!inv){
    return ERROR;
  }

  inv->max_objs = max;

  return OK;
}
/**
 It gets the maximum number of ids of a inventory
*/
int inventory_get_max(Inventory *inv){
  if (!inv){
    return -1;
  }

  return inv->max_objs;
}
/**
 It gets the number of ids of the inventory
*/
int inventory_get_nids(Inventory* inv){
  if(!inv){
    return NO_ID;
  }
  return set_getNids(inv->objects);
}

/**
 It adds an Id to the inventory
 */
STATUS inventory_add( Inventory* inv, Id id){
  if(!inv || !id){
    return ERROR;
  }
  /*Error control*/
  if (inventory_get_nids(inv) >= inv->max_objs){
    return ERROR;
  }
  /*add id to inventory*/
  if(set_add(inv->objects, id)==ERROR){
    return ERROR;
  }
  return OK;
}

/**
 It delete an ID of the inventory
 */
STATUS inventory_delete(Inventory* inv, Id id){
  if(!inv || !id){
    return ERROR;
  }
  /*Error control*/
  if(inventory_get_nids(inv)==0){
    return ERROR;
  }
  /*Delete from inventory*/
  if(set_delete(inv->objects, id)==ERROR){
    return ERROR;
  }
  return OK;
}

STATUS inventory_print(FILE *f, Inventory *inv){
  /*Error control*/
  if (!f || !inv){
    return ERROR;
  }
  /*print inventory IDs*/
  if (set_print(f, inv->objects)==ERROR){
    return ERROR;
  }
  
  
  return OK;
}

/** Finds inventory id*/
BOOL inventory_findId(Inventory* inv, Id id){
  if(!inv || !id){
    return FALSE;
  }
  if(inv->max_objs<=0){
    return FALSE;
  }
  return set_findId(inv->objects, id);
}
