/**
 * @brief It defines the set interface
 *
 * @file set.h
 * @author Enrique Gómez Fernández
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */


#include "types.h"
#include "object.h"
#include "space.h"
#define MAX_SETIDS 5  /*!< Maximum number of sests in the game*/

typedef struct _Set Set;

/**
  * @brief It creates a new set
  * @author Enrique Gómez Fernández
  *
  * set_create allocates memory for a new set
  *  and initializes its members
  * @param id the identification number for the new set
  * @return a new set, initialized
  */
Set* set_create(Id id);

/**
  * @brief It destroys a set
  * @author Enrique Gómez Fernández
  *
  * @param set a pointer to the set
  * @return OK or ERROR if there was a mistake.
  */
STATUS set_destroy(Set* set);

/**
  * @brief It gets the id of a set
  * @author Enrique Gómez Fernández
  *
  * @param set a pointer to the set
  * @return the number of ids or NULL -1 if something goes wrong.
  */
int set_getNids(Set* set);

/**
  * @brief It gets the id of a set
  * @author Enrique Gómez Fernández
  *
  * @param set a pointer to the set
  * @return the id of an set
  */
  

Id* set_getIds(Set* set);

/**
  * @brief It finds an Id the set
  * @author Enrique Gómez Fernández
  *
  * @param set a pointer to the set
  * @param id pointer to the id
  * @return a boolean, specifying if in the set there is an Id (TRUE) or not (FALSE)
  */
BOOL set_findId(Set* set, Id id);
/**
  * @brief It add a id to the set
  * @author Enrique Gómez Fernández
  *
  * @param set a pointer to the set
  * @return OK or ERROR if there was a mistake.
  */
STATUS set_add(Set* set, Id id);
/**
  * @brief It delete the id of a set
  * @author Enrique Gómez Fernández
  *
  * @param set a pointer to the set
  * @param id pointer to the id
  * @return OK or ERROR if there was a mistake.
  */
STATUS set_delete(Set* set, Id id);
/**
  * @brief It prints the set information
  * @author Enrique Gómez Fernández
  *
  * This function prints the number of ids and the ids of the set. 
  *
  * @param set a pointer to the set
  * @return OK or ERROR if there was a mistake.
  */
STATUS set_print(FILE *f,Set *set);
