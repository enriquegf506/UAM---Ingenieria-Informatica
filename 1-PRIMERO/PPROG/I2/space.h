/**
 * @brief It defines the space interface
 *
 * @file space.h
 * @author Profesores PPROG
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */

#ifndef SPACE_H
#define SPACE_H

#include "types.h"
#include "object.h"
#include "set.h"

typedef struct _Space Space;

#define MAX_SPACES 100  /*!< maxumum number of spaces */
#define FIRST_SPACE 1
#define GDESC_ROWS 5    /*!< number of rows of the grafic description */
#define GDESC_COLS 10   /*!< number of columns of the grafic description */

/**
  * @brief It creates a new space, allocating memory and initializing its memebers
  * @author Profesores PPROG
  * 
  * @param id the identification number for the new space
  * @return a new space, initialized
  */
Space* space_create(Id id);

/**
  * @brief It destroys a space, freeing the allocated memory
  * @author Profesores PPROG
  * 
  * @param space a pointer to the space that must be destroyed  
  * @return OK, if everything goes well or ERROR if there was some mistake
  */
STATUS space_destroy(Space* space);

/**
  * @brief It gets the id of a space
  * @author Profesores PPROG
  * 
  * @param space a pointer to the space  
  * @return the id of space
  */
Id space_get_id(Space* space);

/**
  * @brief It sets the name of a space
  * @author Profesores PPROG
  * 
  * @param space a pointer to the space
  * @param name a string with the name to store
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS space_set_name(Space* space, char* name);

/**
  * @brief It gets the name of a space
  * @author Profesores PPROG
  * 
  * @param space a pointer to the space
  * @return  a string with the name of the space
  */
const char* space_get_name(Space* space);

/**
  * @brief It sets the id of the space located at the north
  * @author Profesores PPROG
  * 
  * @param space a pointer to the space
  * @param id the id number of the space located at the north
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS space_set_north(Space* space, Id id);
/**
  * @brief It gets the id of the space located at the north
  * @author Profesores PPROG
  *
  * @param space a pointer to the space
  * @return the id number of the space located at the north 
  */
Id space_get_north(Space* space);

/**
  * @brief It sets the id of the space located at the south
  * @author Profesores PPROG
  *
  * @param space a pointer to the space
  * @param id the id number of the space located at the south
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS space_set_south(Space* space, Id id);
/**
  * @brief It gets the id of the space located at the south
  * @author Profesores PPROG
  *
  * @param space a pointer to the space
  * @return the id number of the space located at the south 
  */
Id space_get_south(Space* space);

/**
  * @brief It sets the id of the space located at the east
  * @author Profesores PPROG
  *
  * @param space a pointer to the space
  * @param id the id number of the space located at the east
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS space_set_east(Space* space, Id id);
/**
  * @brief It gets the id of the space located at the east
  * @author Profesores PPROG
  *
  * @param space a pointer to the space
  * @return the id number of the space located at the east
  */
Id space_get_east(Space* space);

/**
  * @brief It sets the id of the space located at the west
  * @author Profesores PPROG
  *
  * @param space a pointer to the space
  * @param id the id number of the space located at the west
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS space_set_west(Space* space, Id id);
/**
  * @brief It gets the id of the space located at the west
  * @author Profesores PPROG
  *
  * @param space a pointer to the space
  * @return the id number of the space located at the west
  */
Id space_get_west(Space* space);

/**
  * @brief It sets whether the space has an object or not
  * @author Profesores PPROG
  *
  * @param space a pointer to the space
  * @param value a boolean, specifying if in the space there is an object (TRUE) or not (FALSE)
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS space_add_object(Space* space, Id id);

/**
  * @brief It gets whether the space has an object or not
  * @author Profesores PPROG
  *
  * @param space a pointer to the space
  * @return a boolean, specifying if in the space there is an object (TRUE) or not (FALSE)
  */
Set *space_get_objects(Space* space);

/**
  * @brief It prints the space information
  * @author Profesores PPROG
  *
  * This fucntion shows the id and name of the space, the spaces that surrounds it and wheter it has an object or not.
  * @param space a pointer to the space
  * @return OK, if everything goes well or ERROR if there was some mistake
  */
STATUS space_print(Space* space);
/**
  * @brief It determines whether the space has an object or not
  * @author Marcos Alonso
  *
  * @param space a pointer to the space
  * @param id id of the space
  * @return TRUE if the space contains the object, FALSE if not
  */
BOOL space_contain_object(Space* space, Id id);
/**
  * @brief It deletes the object from the space
  * @author Marcos Alonso
  *
  * @param space a pointer to the space
  * @param id id of the space
  * @return TRUE if the space contains the object, FALSE if not
  */
STATUS space_del_object(Space* space, Id id);
/**
  * @brief It sets the space graphic description
  * @author Marcos Alonso
  *
  * @param space a pointer to the space
  * @param new new char for gdesc
  * @param posicion position 
  * @return TRUE or FALSE
  */
STATUS space_set_gdesc(Space* space, char *new, int posicion);
/**
  * @brief It gets the space graphic description
  * @author Marcos Alonso
  *
  * @param space a pointer to the space
  * @param posicion position 
  * @return the graphic description of the space
  */
const char* space_get_gdesc(Space* space, int posicion);


#endif
