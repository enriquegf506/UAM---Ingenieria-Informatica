/**
 * @brief It defines the game loop
 *
 * @file game_loop.c
 * @author Enrique Gómez
 * @version 2.0
 * @date 30-11-2020
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphic_engine.h"
#include "game.h"
#include "command.h"
#include "game_reader.h"

/**
   * @brief inicilizes the game and verify it doesn't have errors
   * @author Enrique Gómez
   *
   * 
   * @param *game game variable used to create the game
   * @param **gengine game engine
	 * @param *file_name game of the file by which we create the game
	 * @return 1 if error, 0 if there are not errors
   */
int game_loop_init(Game *game, Graphic_engine **gengine, char *file_name, unsigned long i);
/**
   * @brief execute the game
   * @author Enrique Gómez 
   * 
   * @param game variable that contains the game
   * @param *gengine motor used for the game graphics
   */
void game_loop_run(Game game, Graphic_engine *gengine);
/**
   * @brief destrois the game and the graphics created
   * @author Enrique Gómez
   *
   * 
   * @param *gengine the engine of the graphics we delete
   * @param game the game we delete
   */
void game_loop_cleanup(Game game, Graphic_engine *gengine);


int main(int argc, char *argv[])
{
  Game game;
  Graphic_engine *gengine;
  unsigned long   i = 0;
  srand(time(NULL));
  if (argc < 2)
  {
    fprintf(stderr, "Use: %s <game_data_file>\n", argv[0]);
    return 1;
  }
 
  if (!game_loop_init(&game, &gengine, argv[1], i))
  {
    game_loop_run(game, gengine);
    game_loop_cleanup(game, gengine);
    i++;
  }

  return 0;
}

/**
  inicilizes the game and verify it doesn't have errors
*/
int game_loop_init(Game *game, Graphic_engine **gengine, char *file_name, unsigned long i)
{
  if (game_create_from_file(game, file_name, i) == ERROR)
  {
    fprintf(stderr, "Error while initializing game.\n");
    return 1;
  }

  if ((*gengine = graphic_engine_create()) == NULL)
  {
    fprintf(stderr, "Error while initializing graphic engine.\n");
    game_destroy(game);
    return 1;
  }

  return 0;
}

/**
  execute the game
*/
void game_loop_run(Game game, Graphic_engine *gengine)
{
  T_Command command = NO_CMD;
  graphic_engine_paint_game(gengine, &game);

  while ((command != EXIT) && !game_is_over(&game))
  {
    command = command_get_user_input();
    game_update(&game, command);
    graphic_engine_paint_game(gengine, &game);
  }
}

/**
  destroys the game and the graphics created
*/
void game_loop_cleanup(Game game, Graphic_engine *gengine)
{
  game_destroy(&game);
  graphic_engine_destroy(gengine);
}
