/** 
 * @brief It defines the game reader
 * 
 * @file game_reader.h
 * @author Marcos Alonso
 * @version 2.0 
 * @date 21-02-2022
 * @copyright GNU Public License
 */

#ifndef GAME_READER_H
#define GAME_READER_H

typedef struct _Game Game;
#include "types.h"

/**
   * @brief load the spaces using hormiguero.h
   * @author Marcos Alonso
   *
   * 
   * @param game the game that contains the data
   * @param filename the file name
   * @return the status
   */
STATUS game_load_spaces(Game *game, char *filename);

/** 
 * @brief It load the objects.
 * @author Marcos Alonso
 * @param game a pointer to the game
 * @param filename char pointer.
 * game_load_objects open a file to read it.
 * 
 * @return status, if everything goes well or ERROR if there was some mistake opening the file.

 */
STATUS game_load_objects(Game *game, char *filename);
/** 
 * @brief It load the links.
 * @author Marcos Alonso
 * @param game a pointer to the game
 * @param filename char pointer.
 * game_load_links open a file to read it.
 * 
 * @return status, if everything goes well or ERROR if there was some mistake opening the file.

 */
STATUS game_load_links(Game *game, char *filename);

/** 
 * @brief It load the player.
 * @author Marcos Alonso
 * @param game a pointer to the game
 * @param filename char pointer.
 * 
 * @return status, if everything goes well or ERROR if there was some mistake opening the file.

 */
STATUS game_load_players(Game *game, char *filename);

/** 
 * @brief It load the enemies.
 * @author Marcos Alonso
 * @param game a pointer to the game
 * @param filename char pointer.
 * 
 * @return status, if everything goes well or ERROR if there was some mistake opening the file.

 */
STATUS game_load_enemies(Game *game, char *filename);
#endif
