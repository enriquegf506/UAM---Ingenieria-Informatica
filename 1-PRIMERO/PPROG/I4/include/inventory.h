/**
 * @brief It defines the inventory interface
 *
 * @file inventory.h
 * @author Enrique Gómez 
 * @version 2.0
 * @date 27-02-2022
 * @copyright GNU Public License
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "types.h"
#include "set.h"

#define MAX_PORT 4 /*num of objects the player can take*/

/** Structure for the inventory*/
typedef struct _Inventory Inventory;


/**
  * @brief It creates an Inventory
  * @author Enrique Gómez
  *
  * enemy_create allocates memory for a new inventory 
  *  
  * @return a new enemy initialized if everything goes well or NULL if there was some mistake
  */
Inventory* inventory_create();

/**
  * @brief It destroys an inventory
  * @author Enrique Gómez Fernández
  *
  * @param inv a pointer to the inventory
  * @return OK or ERROR if there was a mistake.
  */
STATUS inventory_destroy(Inventory* inv);
/**
  * @brief It sets maximum number of ids of a inventory
  * @author Enrique Gómez Fernández
  *
  * @param inv a pointer to the inventory
  * @param max number of maximum ids wanted to set
  * @return OK or ERROR if there was a mistake.
  */
STATUS inventory_set_max(Inventory *inv, int max);
/**
  * @brief It gets maximum number of ids of a inventory
  * @author Enrique Gómez Fernández
  *
  * @param inv a pointer to the inventory
  * @return the number of ids or NULL -1 if something goes wrong.
  */
int inventory_get_max(Inventory *inv);
/**
  * @brief It gets the number of id of a inventory
  * @author Enrique Gómez Fernández
  *
  * @param inv a pointer to the inventory
  * @return the number of ids or NULL -1 if something goes wrong.
  */
int inventory_get_nids(Inventory* inv);
/**
  * @brief It add a id to the inventory
  * @author Enrique Gómez Fernández
  *
  * @param inv a pointer to the inventory
  * @param id the id of the inventory
  * @return OK or ERROR if there was a mistake.
  */
STATUS inventory_add( Inventory* inv, Id id);
/**
  * @brief It delete the id of a inventory
  * @author Enrique Gómez Fernández
  *
  * @param inv a pointer to the inventory
  * @param id pointer to the id
  * @return OK or ERROR if there was a mistake.
  */
STATUS inventory_delete(Inventory* inv, Id id);
/**
  * @brief It prints the inventory information
  * @author Enrique Gómez Fernández
  *
  * This function prints the number of ids and the ids of the inventory. 
  *
  * @param inv a pointer to the inventory
  * @param f a pointer to the file
  * @return OK or ERROR if there was a mistake.
  */
STATUS inventory_print(FILE *f, Inventory *inv);
/**
  * @brief It find an id in the inventory
  * @author Enrique Gómez Fernández
  *
  * @param inv a pointer to the inventory
  * @param id pointer to the id
  * @return TRUE or FALSE if there wasn't the id in the inventory.
  */
BOOL inventory_findId(Inventory* inv, Id id);


#endif
