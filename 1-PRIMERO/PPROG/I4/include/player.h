/**
 * @brief Funcionalidad para el manejo de los jugadores
 *
 * @file player.h
 * @author Iñigo Alvarez
 * @version 2.0
 * @date 16-02-2022
 * @copyright GNU Public License
 */

#ifndef PLAYER_H
#define PLAYER_H

#define PL_HEALTH 5 /*!< Number of health the player starts with*/
#include "types.h"
#include "object.h"
#include "inventory.h"

/** Structure for the player*/
typedef struct _Player Player;

/**
  * @brief It creates a new player
  * @author Iñigo Alvarez
  *
  * player_create allocates memory for a new player
  *  and initializes its members
  * @param id the identification number for the new player
  * @return a new player, initialized
  */
Player* player_create(Id id);


/**
  * @brief It destroys a player
  * @author Iñigo Alvarez
  *
  * player_destroy frees the previous memory allocation 
  *  for a player
  * @param player a pointer to the player that must be destroyed  
  * @return OK, if everything goes well or ERROR if there was some mistake
  */
STATUS player_destroy(Player* player);

/**
  * @brief It gets the id of a player
  * @author Iñigo Alvarez
  * 
  * @param player a pointer to the player  
  * @return the id of a player
  */
Id player_get_id(Player* player);

/**
  * @brief It sets the id of the player location
  * @author Iñigo Alvarez
  * 
  * @param player a pointer to the player
  * @param id the id number of the player location
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS player_set_location(Player* player, Id id);

/**
  * @brief It gets the id of the player location
  * @author Iñigo Alvarez
  *
  * @param player a pointer to the player
  * @return the id number of the player location
  */
Id player_get_location(Player* player);

/**
  * @brief It sets the name of a player
  * @author Iñigo Alvarez
  * 
  * @param player a pointer to the player
  * @param name a string with the name to store
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS player_set_name(Player* player, char* name);


/**
  * @brief It gets the name of a player
  * @author Iñigo Alvarez
  * 
  * @param player a pointer to the player
  * @return  a string with the name of the player
  */
const char* player_get_name(Player* player);


/**
  * @brief It gets whether the player has an object or not
  * @author Iñigo Alvarez
  *
  * @param player a pointer to the player
  * @return a boolean, specifying if the player has an object (TRUE) or not (FALSE)
  */
Inventory *player_get_objects(Player* player);

/**
  * @brief It prints the player information
  * @author Iñigo Alvarez
  *
  * This fucntion shows the id, the name, the location and object id of the player
  * @param player a pointer to the player
  * @return OK, if everything goes well or ERROR if there was some mistake
  */
STATUS player_print(Player* player);

/**
  * @brief It gets the player's health
  * @author Iñigo Alvarez
  *
  * @param player a pointer to the player
  * @return enemy's health or NULL  if something goes wrong and the enemy's health 
  */
int player_get_health(Player* player) ;

/**
  * @brief It sets the player's health
  * @author Iñigo Alvarez
  *
  * @param player a pointer to the player
  * @param health the new health of the player
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS player_set_health(Player* player, int health);

/**
  * @brief It determines if the player has an object
  * @author Iñigo Alvarez
  *
  * @param player a pointer to the player
  * @param id the player id
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS player_del_object(Player* player, Id id);

/**
  * @brief It determines if the player has an object
  * @author Iñigo Alvarez
  *
  * @param player a pointer to the player
  * @param id the player id
  * @return TRUE or FALSE
  */
BOOL player_contain_object(Player* player, Id id);

/**
  * @brief It adds an object to the player
  * @author Iñigo Alvarez
  *
  * @param player a pointer to the player
  * @param id the player id
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS player_add_object(Player* player, Id id);

/**
  * @brief It sets the maximun objects for the player
  * @author Iñigo Alvarez
  *
  * @param player a pointer to the player
  * @param max tha maximun objects it can have
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
STATUS player_set_max(Player *player, int max);

/**
  * @brief It gets the maximun objects the player has
  * @author Iñigo Alvarez
  *
  * @param player a pointer to the player
  * @return OK, if everything goes well or ERROR if there was some mistake 
  */
int player_get_max(Player *player);
#endif
