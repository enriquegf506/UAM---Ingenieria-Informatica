/** 
 * @brief It defines the game interface
 * 
 * @file game.h
 * @author Daniel Aquino Y Enrique Gomez
 * @version 2.0 
 * @date 29-11-2021 
 * @copyright GNU Public License
 */

#ifndef GAME_H
#define GAME_H

#include "command.h"
#include "space.h"
#include "types.h"
#include "object.h"
#include "player.h"

typedef struct _Game{
  Player* player;
  Object* object;
  Space* spaces[MAX_SPACES + 1];
  T_Command last_cmd;
} Game;

/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to the game
 * game_create allocates memory for a new game
 * and initializes its members
 * 
 * @return OK, if everything goes well or ERROR if there was some mistake 

 */

STATUS game_create(Game *game);
/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to the game
 * @param filename a pointer to the file name
 * 
 * @return OK, if everything goes well or ERROR if there was some mistake 

 */

STATUS game_create_from_file(Game *game, char *filename);
/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to the game
 * @param cmd 
 * 
 * @return OK, if everything goes well or ERROR if there was some mistake 

 */

STATUS game_update(Game *game, T_Command cmd);
/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to game
 * 
 * @return OK, if everything goes well or ERROR if there was some mistake 

 */

STATUS game_destroy(Game *game);
/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to the game
 * 
 * @return True if the game is over or False if the game is not over

 */

BOOL game_is_over(Game *game);
/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to the game
 */

void game_print_data(Game *game);

/** 
 * @brief It prints the game screen
 * @author Enrique Gómez
 * @param game a pointer to the game
 * 
 * @return OK if everything goes whrigt or ERROR if not

 */
void   game_print_screen(Game* game);


/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to the game
 * @param
 * 
 * @return The space it gets

 */

Space *game_get_space(Game *game, Id id);
/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to the game
 * 
 * @return Id of the player location

 */
Id game_get_player_location(Game *game);
/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to the game
 * 
 * @return the Id of the object location

 */

Id game_get_object_location(Game *game);
/** 
 * @brief It defines the game interface
 * @author Daniel Aquino 
 * @param game a pointer to the game
 * 
 * @return The las command

 */

T_Command game_get_last_command(Game *game);

/** 
 * @brief It add a space to the game
 * @author Enrique Gómez 
 * @param game a pointer to the game
 * @param space a pointer to the space
 * 
 * @return OK, if everything goes well or ERROR if there was some mistake 
 */

STATUS game_add_space(Game *game, Space *space);
#endif
