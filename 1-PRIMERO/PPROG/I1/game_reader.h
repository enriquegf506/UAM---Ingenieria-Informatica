/** 
 * @brief It defines the game interface
 * 
 * @file game.h
 * @author Enrique Gómez
 * @version 2.0 
 * @date 16-02-2021 
 * @copyright GNU Public License
 */

#ifndef GAME_READER_H
#define GAME_READER_H


#include "game.h"
#include "space.h"


STATUS game_load_spaces(Game* game, char* filename);

#endif