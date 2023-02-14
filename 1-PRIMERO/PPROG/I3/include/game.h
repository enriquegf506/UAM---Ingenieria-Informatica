/** 
 * @brief It defines the game interface
 * 
 * @file game.h
 * @author Iñigo Alvarez
 * @version 2.0 
 * @date 29-11-2021 
 * @copyright GNU Public License
 */


#ifndef GAME_H
#define GAME_H

#include "game_reader.h"
#include "command.h"
#include "space.h"
#include "types.h"
#include "player.h"
#include "object.h"
#include "link.h"
#include "enemy.h"
#include "set.h"
#include "game_reader.h"

#define MAX_RAND 9 /*!< To get a random number between 0 and 9*/

/**
   * @brief inicializes the game
   * @author Iñigo Alvarez
   *
   * 
   * @return OK or ERROR
   */
Game *game_create();

/**
   * @brief create the game using hormiguero.h
   * @author Marcos Alonso
   *
   * 
   * @param filename the file name
   * @return the status
   */
Game *game_create_from_file(char *filename);

/**
   * @brief actualizes the game depending on the command passed
   * @author Iñigo Alvarez
   *
   * 
   * @param game contains the data game
   * @param cmd el comamand we need to use to actualize
   * @return OK or ERROR
   */
STATUS game_update(Game *game, T_Command cmd);

/**
   * @brief destrois the game space
   * @author Iñigo Alvarez
   *
   * 
   * @param game contains the spaces of the game to be delete
   * @return OK or ERROR
   */
STATUS game_destroy(Game *game);

/**
   * @brief Determines the game has finished
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that ends
   * @return FALSE
   */
BOOL game_is_over(Game *game);

/**
   * @brief prints the data game
   * @author Iñigo Alvarez
   *
   * 
   * @param game contains the game data
   */
void game_print_data(Game *game);

/**
   * @brief obtains the game space that corresponds with the id
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains all the data
   * @param id the id of the space is searching
   * @return the space searched
   */
Space *game_get_space(Game *game, Id id);

/**
   * @brief it says the last command
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contais the data
   * @return the last command
   */
T_Command game_get_last_command(Game *game);

/**
   * @brief adds an space to the game
   * @author Iñigo Alvarez
   *
   * 
   * @param game el game that contains the data
   * @param space el space that is added
   * @return OK or ERROR
   */
STATUS game_add_space(Game *game, Space *space);

/**
   * @brief oobtains the location of the space
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param position position at the moment
   * @return la id of the location in which we are
   */
Id game_get_space_id_at(Game *game, int position);

/**
   * @brief adds an obj to the game
   * @author Iñigo Alvarez
   *
   * 
   * @param game el game that contains the data
   * @param obj el obj that is added
   * @return OK or ERROR
   */
STATUS game_add_obj(Game *game, Object *obj);
/**
   * @brief adds an enemy to the game
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param enemy the enemy that is added
   * @return OK or ERROR
   */
STATUS game_add_enemy(Game *game, Enemy *enemy);
/**
   * @brief adds a player to the game
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param player the player that is added
   * @return OK or ERROR
   */
STATUS game_add_player(Game *game, Player *player);
/**
   * @brief adds a link to the game
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param link the link that is added
   * @return OK or ERROR
   */
STATUS game_add_link(Game *game, Link *link);
/**
   * @brief Gets the connection link status
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param id the id of the link
   * @param direction the new direction
   * @return a LINKSTATUS
   */
LINKSTATUS  game_get_connection_status(Game *game, Id id, DIRECTION direction);
/**
   * @brief Gets the game connection links
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param id the id of the link
   * @param direction the new direction
   * @return an ID
   */
Id game_get_connection(Game *game, Id id, DIRECTION direction);
/**
   * @brief Gets the game link
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param origin the origin of the link
   * @param direction the new direction of the link
   * @return a link
   */
Link *game_get_link(Game *game, Id origin, DIRECTION direction);
/**
   * @brief Gets the game player
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @return the game player
   */
Player *game_get_player(Game *game);
/**
   * @brief Gets the game enemy
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @return the game enemy
   */
Enemy *game_get_enemy(Game *game);
/**
   * @brief sets the game description
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param description char with the game description
   * @return OK or ERROR
   */
STATUS game_set_description(Game *game, char *description);
/**
   * @brief Gets the game description
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @return the game description char
   */
char *game_get_description(Game *game);
/**
   * @brief sets the player location in the game
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param id new id
   * @return OK or ERROR
   */
STATUS game_set_player_location(Game* game, Id id);
/**
   * @brief Gets the game object
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param id the id of the object we want to get
   * @return an object
   */
Object *game_get_obj(Game *game, Id id);
/**
   * @brief Gets the game state of the command
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @return OK or ERROR
   */
STATUS game_get_state(Game *game);
/**
   * @brief Sets the game state of the command
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that contains the data
   * @param state the status we want to set
   * @return OK or ERROR
   */
STATUS game_set_state(Game *game, STATUS state);
#endif
