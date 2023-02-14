/**
 * @brief It defines the object interface
 *
 * @file object.h
 * @author Enrique Gómez
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "types.h"

/** Structure for the object*/
typedef struct _Object Object;

# define MAX_OBJECTS 4  /*!< Maximum number of object possible*/


/**
  * @brief It creates a new object
  * @author Enrique Gómez
  *
  * object_create allocates memory for a new object
  * @param id the identification number for the new object
  * @return a new object, initialized
  */
Object* object_create(Id id);

/**
  * @brief It destroys an object
  * @author Enrique Gómez
  *
  * object_destroy frees the previous memory allocation 
  *  for an object
  * @param object a pointer to the object that must be destroyed  
  * @return OK, if everything goes well or ERROR if there was some mistake
  */
STATUS object_destroy(Object* object);

/**
  * @brief It gets the id of an object
  * @author Enrique Gómez
  * 
  * @param object a pointer to the object  
  * @return the id of object
  */
Id object_get_id(Object* object);

/**
  * @brief It sets the name of an object
  * @author Enrique Gómez
  * 
  * @param object a pointer to the object
  * @param id the id of an object
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS object_set_id(Object *object, Id id);

/**
  * @brief It sets the name of an object
  * @author Enrique Gómez
  * 
  * @param object a pointer to the object
  * @param name a string with the name to store
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS object_set_name(Object* object, char* name);

/**
  * @brief It gets the name of an object
  * @author Enrique Gómez
  * 
  * @param object a pointer to the object
  * @return  a string with the name of the object
  */
const char* object_get_name(Object* object);

/**
  * @brief It prints an object data
  * @author Enrique Gómez
  * 
  * @param object a pointer to the object
  * @return  a status
  */
STATUS object_print(Object* object);

/**
  * @brief It sets the object's description
  * @author Enrique Gómez
  * 
  * @param object a pointer to the object
  * @param description the new description
  * @return  a status
  */
STATUS object_set_description(Object* object, char* description);

/**
  * @brief It gets the object's description
  * @author Enrique Gómez
  * 
  * @param object a pointer to the object
  * @return the object description
  */
char *object_get_description(Object *object);

#endif
