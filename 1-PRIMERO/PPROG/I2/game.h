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

#include "command.h"
#include "space.h"
#include "types.h"
#include "player.h"
#include "object.h"
#include "enemy.h"
#include <time.h>

#define HEALTH 10    /*!< Health of the player and enemy*/ 
#define MAX_RAND 10  /*!< Maximum random number we can get for the command attack*/ 

/**
 * @brief Game
 *
 * This struct stores all the information of the game module.
 */
typedef struct _Game
{
  Player *pl;                 /*!< pl pointer to player, the player of the game*/ 
  Object *obj[MAX_OBJECTS];   /*!< Array of pointers to the object */
  Space *spaces[MAX_SPACES];  /*!< Array of pointers to the space */
  T_Command last_cmd;         /*!< Last comand used */
  Enemy *enemy;               /*!< enemy pointer to enemy, the enemy of the game */
  STATUS state;                 /*!< The state of the command*/
} Game;

/**
   * @brief inicializes the game
   * @author Iñigo Alvarez
   *
   * 
   * @param game the game that is inicialized
   * @return OK or ERROR
   */
STATUS game_create(Game *game);

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
   * @param space el obj that is added
   * @return OK or ERROR
   */
STATUS game_add_obj(Game *game, Object *obj);

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
