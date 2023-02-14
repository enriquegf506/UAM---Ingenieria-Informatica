/** 
 * @brief It defines the game interface
 * 
 * @file game.h
 * @author Daniel Aquino Y Enrique Gomez
 * @version 2.0 
 * @date 29-11-2021 
 * @copyright GNU Public License
 */

#ifndef SET_H
#define SET_H

#include "types.h"


typedef struct _Set Set;

Set *set_create(Set *set);
STATUS set_destroy(Set *set);
STATUS set_add(Set *set);
STATUS set_delete(Set *set);
STATUS set_print(Set *set);













#endif