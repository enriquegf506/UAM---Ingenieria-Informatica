/**
 * @brief It defines the link interface
 *
 * @file link.h
 * @author Iñigo Alvarez
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */

#ifndef LINK_H
#define LINK_H

#include "types.h"
#include "object.h"
#define MAX_LINKS 50  /*!< Maximum number of links possible*/


typedef struct _Link Link; /*!< Structure for the link*/

/**
  * @brief It creates a new link, allocating memory and initializing it
  * @author Iñigo Alvarez
  * 
  * @param id the identification number for the new link
  * @return a new link, initialized
  */
Link* link_create(Id id);

/**
  * @brief link_destroy frees the previous memory allocation for a link
  *
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link that must be destroyed
  * @return OK, if everything goes well or ERROR if there was some mistake
  */
STATUS link_destroy(Link* link);

/**
  * @brief It gets the id of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link  
  * @return the id of link
  */
Id link_get_id(Link* link);

/**
  * @brief It sets the name of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the space
  * @param name a string with the name to store
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS link_set_name(Link* link, char* name);

/**
  * @brief It gets the name of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link
  * @return  a string with the name of the link
  */
const char* link_get_name(Link* link);

/**
  * @brief It sets the origin of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link
  * @param id the new id for the origin
  * @return  OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS link_set_origin(Link* link, Id id);

/**
  * @brief It gets the origin of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link  
  * @return the id of the origin
  */
Id link_get_origin(Link* link);

/**
  * @brief It sets the destination of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link
  * @param id the new id for the destination
  * @return  OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS link_set_dest(Link* link, Id id);

/**
  * @brief It gets the destination of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link  
  * @return the id of the destination
  */
Id link_get_dest(Link* link);

/**
  * @brief It sets the state of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link
  * @param state the new state for the destination
  * @return  OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS link_set_state(Link* link, LINKSTATUS state);

/**
  * @brief It gets the state of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link  
  * @return the state of the link
  */
LINKSTATUS link_get_state(Link* link);

/**
  * @brief It sets the direction of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link
  * @param direction the new direction
  * @return  OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS link_set_direction(Link* link, DIRECTION diretion);

/**
  * @brief It gets the direction of a link
  * @author Iñigo Alvarez
  * 
  * @param link a pointer to the link  
  * @return the direction of the link
  */
DIRECTION link_get_direction(Link* link);

/**
  * @brief It prints the link information
  * @author Iñigo Alvarez
  *
  * This fucntion shows the id and name of the link, the direction, state, origin and destination.
  * @param link a pointer to the link
  * @return OK, if everything goes well or ERROR if there was some mistake
  */
STATUS link_print(Link* link);

Id link_get_dest(Link* link);

#endif
