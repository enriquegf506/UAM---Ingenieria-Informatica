/**
 * @brief It defines the object interface
 *
 * @file object.h
 * @author Daniel Aquino Santiago
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */


#ifndef OBJECT_H
#define OBJECT_H


#include "types.h"


#define OBJECT_ID 45

typedef struct _Object Object;

Object* object_create(Id id);
STATUS object_destroy(Object* object);
Id object_get_id(Object* object);
STATUS object_set_id(Object* object, Id id);
STATUS object_set_name(Object* object, char* name);
const char* object_get_name(Object* object);
STATUS object_print(Object* object);

#endif