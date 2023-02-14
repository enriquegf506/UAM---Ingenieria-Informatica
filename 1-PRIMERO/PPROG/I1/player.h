/**
 * @brief It defines the player interface
 *
 * @file player.h
 * @author Enrique Gómez 
 * @version 2.0
 * @date 19-02-2022
 * @copyright GNU Public License
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"


#define PLAYER_ID 1

typedef struct _Player Player;

Player* player_create(Id id);
STATUS player_destroy(Player* player);
Id player_get_id(Player* player);
STATUS player_set_id(Player* player, Id id);
STATUS player_set_name(Player* player, char* name);
const char* player_get_name(Player* player);
Id player_get_location(Player* player);
STATUS player_set_location(Player* player, Id id);
Id player_get_object_id(Player* player);
STATUS player_set_object_id(Player* player, Id id);
STATUS player_print(Player* player);

#endif