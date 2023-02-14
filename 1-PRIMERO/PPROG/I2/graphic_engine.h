/**
 * @brief It defines the textual graphic engine interface
 *
 * @file graphic_engine.h
 * @author Marcos Alonso
 * @version 2.0
 * @date 29-11-2021
 * @copyright GNU Public License
 */

#ifndef __GRAPHIC_ENGINE__
#define __GRAPHIC_ENGINE__

#include "game.h"

typedef struct _Graphic_engine Graphic_engine;

/**
  * @brief cretes a graphic_engine structure
  * @author Marcos Alonso
  *
  * creates a structure Graphic_engine and inicilizes it with a new area inside the screen 
  * 
  * @return la estructure inicialized
  */
Graphic_engine *graphic_engine_create();

/**
  * @brief delete the content of the graphic_engine structure
  * @author Marcos Alonso
  *
  * delete the content
  * 
  * @param ge a Graphic_engine structure we want to reset
  */
void graphic_engine_destroy(Graphic_engine *ge);

/**
  * @brief prints the spaces and the graphics of the game
  * @author Marcos Alonso
  *
  * create all the game graphics: map, ant, foof...
  * 
  * @param ge estructure used to create the graphics
  * @param game estructure used for the data game
  */
void graphic_engine_paint_game(Graphic_engine *ge, Game *game);

#endif
